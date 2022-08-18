/*
    Description: Use GoPLUS Module for four-channel servo test and three-channel ADC test.
*/
#include <M5Stack.h>
#include <WiFiManager.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <Wire.h>
#include "GoPlus2.h"
#include <math.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <AsyncTCP.h>
#include <AsyncUDP.h>
#include <ESPAsyncWebServer.h>
//#include "MFRC522_I2C.h"
#include <time.h>
#include <NTPClient.h>
//#include <MFRC522.h>
#include "MFRC522_I2C.h"
#define FORMAT_FILESYSTEM false
#define N 10


WiFiManager wm;   // Définition du nom du WifiManager = wm
const char* ssid = "LUIGI_WIFI";  // ssid (exemple)
const char* password = "mariokartmindstorm";  // Mot de passe (exemple)
unsigned long temps_lecture_tag;
char* sender;
char* receiver;
int nbJoueurs =1;
int nbCheckpoints = 9;
GoPlus2 goPlus;
MFRC522 mfrc522(0x28);

#define X_LOCAL 40
#define Y_LOCAL 30

#define X_OFFSET 160
#define Y_OFFSET 23
#define MAXSPEED 127
#define LIMITTURN 0.01

//*************************Partie Fatine*********************************

// Define NTP Client to get time
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP,"fr.pool.ntp.org", 3600, 1000);
IPAddress IP_Courante;

char* ids_CP1[7]={"b3d4fe2a","6c4c312","36e6e3ad","16bd5b13","70b670a5","56f6ffad","563f6dc"}; 
char* ids_CP2[7]={"562a36c6","3372522b","96434b73","c342792b","863a48af","13d76f2b","43b14a2b"};
char* ids_CP3[7]={"96fb7513","63b48f24","f38a562b","6c7efad","e6c452c","86b0ed12","f68bd0c"};
char* objets_ids[4]={"a6115f13","86c92b65","e72e7775","90fd81a5"};
String chaine_a_envoyer = "";
int IPs_Liste[N]; //faut ajouter toutes les adresses Ip des karts à l'écoute 
int rank[N];    //initialisation du tablea ude classement
//il faut que tu trouve l'indice i de l'ip courante 
unsigned long  etat_course[N][100]; //un tableau qui donne pour chaque adresse IP et chaque checkpoint la valeur du compteur
//pour les colonnes du tableau : indice 0:CP1 indice 1:CP2 indice 2:CP3 
int NbCP;//le nombre de checkpoints passés

//Fonction qui verifie si un string est present dans un tableau



bool Contains_Tab(char** tab,char * str,int taille){
  for(int i=0;i<taille;i++){
    if(strcmp(str,tab[i])==0){

      return true;
    }
  }
  return false;
}

bool Contains_Tabint(int* tab,int str,int taille){
  for(int i=0;i<taille;i++){
    if(str==tab[i]){
      //Serial.println("Contains_Tab retourne la valeur true");
      /*
      Serial.print(str);
      Serial.print(" egale a ");
      Serial.println(tab[i]);
      */
      return true;
    }else{
      /*
      Serial.print(str);
      Serial.print(" different de ");
      Serial.println(tab[i]);
      */
    }
  }
  //Serial.println("Contains_Tab retourne la valeur false");
  return false;
}
int getRank(){
  Serial.println(" Ma liste Rank");
  for(int i=0;i<nbJoueurs;i++){
    Serial.print(" Rank : ");
    Serial.print(i);
    Serial.print(" Ip : ");
    Serial.print(rank[i]);
    Serial.print(" |");
  }
  Serial.println("");
  for(int i=0;i<nbJoueurs;i++){
    Serial.println(i);
    Serial.println(WiFi.localIP()[3]);
    Serial.println(rank[i]);
    if(WiFi.localIP()[3]==rank[i]){
      return i;
    }
  }
  return 404;
}

int Find_Tab(char ** tab,char * str){
  for(int i=0;i<sizeof(tab);i++){
    if(strcmp(str,tab[i])==0){
      return i;
    }
  }
  return -1;    //surement un probleme avec les Strings si jamais ce return est utilisé
}
int Find_Pred(){//retourne le kart devant soi
 
    for(int i=0;i<nbJoueurs;i++){
      if(rank[i]==WiFi.localIP()[3]){
        if(i==0){
          return rank[nbJoueurs-1];
        }else{
          return rank[i-1];
        }
      }
    }
  } 


//*************************Fin Partie Fatine*******************************

typedef struct{ 
    int ack;
    String donnee;
    int nbEnvoi;
    IPAddress ip;
    unsigned long myTime;
  } Message;
int maxRepetition=5;                //nb max de renvoi avant abandon
int sequence =1;
int maxqueue =100;
int messagesSize =0;
Message messages[100];
char * messCarapaceRouge="cr";
char * messCarapaceVerte ="cv";
char * messCheckPoint="cp";
unsigned long rtt = 2000;           //RTT de 2 secondes
unsigned long paralizedAt=millis();
unsigned long tempsParalized = 3000;  //durée de l'imobilisation
boolean paralized =false;
int probaCarapaceVerte = 50;
char* objets[2] = {"carapaceRouge","carapaceVerte"};
int nbObjets =2;



//******************************************

size_t received = 0;

int speed1 = 0;
int speed2 = 0;

int flag = 0;
int num = 0;
String RFIDvalue;
String lastRFID;
int LapNB=0;
int checkPointNB=0;
AsyncUDP udp;

AsyncWebServer server(80);
AsyncWebSocket ws("/ws");


const char index_html[] PROGMEM = R"rawliteral(
<html lang="fr">
<head>
    <meta charset="utf-8">
    <meta name="viewport" content="width=device-width; height=device-height; initial-scale=1.0; maximum-scale=1.0;" />
    <title>Video game controller with a simple spaceship game - using HandJS</title>
  <script>
  (function(){if(!Array.prototype.indexOf){Array.prototype.indexOf=function(e){var t=Object(this);var n=t.length>>>0;if(n===0){return-1}var r=0;if(arguments.length>0){r=Number(arguments[1]);if(r!=r){r=0}else if(r!=0&&r!=Infinity&&r!=-Infinity){r=(r>0||-1)*Math.floor(Math.abs(r))}}if(r>=n){return-1}var i=r>=0?r:Math.max(n-Math.abs(r),0);for(;i<n;i++){if(i in t&&t[i]===e){return i}}return-1}}var e=["PointerDown","PointerUp","PointerMove","PointerOver","PointerOut","PointerCancel","PointerEnter","PointerLeave","pointerdown","pointerup","pointermove","pointerover","pointerout","pointercancel","pointerenter","pointerleave"];var t="touch";var n="pen";var r="mouse";var i={};var s=function(e){while(e&&e.handjs_forcePreventDefault!==true){e=e.parentElement}return e!=null};var o=function(e,i){var o;if(document.createEvent){o=document.createEvent("MouseEvents");o.initMouseEvent(i,true,true,window,1,e.screenX,e.screenY,e.clientX,e.clientY,e.ctrlKey,e.altKey,e.shiftKey,e.metaKey,e.button,null)}else{o=document.createEventObject();o.screenX=e.screenX;o.screenY=e.screenY;o.clientX=e.clientX;o.clientY=e.clientY;o.ctrlKey=e.ctrlKey;o.altKey=e.altKey;o.shiftKey=e.shiftKey;o.metaKey=e.metaKey;o.button=e.button}if(o.offsetX===undefined){if(e.offsetX!==undefined){if(Object&&Object.defineProperty!==undefined){Object.defineProperty(o,"offsetX",{writable:true});Object.defineProperty(o,"offsetY",{writable:true})}o.offsetX=e.offsetX;o.offsetY=e.offsetY}else if(e.layerX!==undefined){o.offsetX=e.layerX-e.currentTarget.offsetLeft;o.offsetY=e.layerY-e.currentTarget.offsetTop}}if(e.isPrimary!==undefined)o.isPrimary=e.isPrimary;else o.isPrimary=true;if(e.pressure)o.pressure=e.pressure;else{var u=0;if(e.which!==undefined)u=e.which;else if(e.button!==undefined){u=e.button}o.pressure=u==0?0:.5}if(e.rotation)o.rotation=e.rotation;else o.rotation=0;if(e.hwTimestamp)o.hwTimestamp=e.hwTimestamp;else o.hwTimestamp=0;if(e.tiltX)o.tiltX=e.tiltX;else o.tiltX=0;if(e.tiltY)o.tiltY=e.tiltY;else o.tiltY=0;if(e.height)o.height=e.height;else o.height=0;if(e.width)o.width=e.width;else o.width=0;o.preventDefault=function(){if(e.preventDefault!==undefined)e.preventDefault()};if(o.stopPropagation!==undefined){var a=o.stopPropagation;o.stopPropagation=function(){if(e.stopPropagation!==undefined)e.stopPropagation();a.call(this)}}o.POINTER_TYPE_TOUCH=t;o.POINTER_TYPE_PEN=n;o.POINTER_TYPE_MOUSE=r;o.pointerId=e.pointerId;o.pointerType=e.pointerType;switch(o.pointerType){case 2:o.pointerType=o.POINTER_TYPE_TOUCH;break;case 3:o.pointerType=o.POINTER_TYPE_PEN;break;case 4:o.pointerType=o.POINTER_TYPE_MOUSE;break}if(e.currentTarget&&s(e.currentTarget)===true){o.preventDefault()}if(e.target){e.target.dispatchEvent(o)}else{e.srcElement.fireEvent("on"+h(i),o)}};var u=function(e,t){e.pointerId=1;e.pointerType=r;o(e,t)};var a=function(e,n,r,i){var s=n.identifier+2;n.pointerId=s;n.pointerType=t;n.currentTarget=r;n.target=r;if(i.preventDefault!==undefined){n.preventDefault=function(){i.preventDefault()}}o(n,e)};var f=function(e,t){while(e&&!(e.__handjsGlobalRegisteredEvents&&e.__handjsGlobalRegisteredEvents[t])){e=e.parentElement}return e!=null};var l=function(e,t,n,r){if(f(n,e)){a(e,t,n,r)}};var c=function(e,t,n,r){if(e.preventManipulation)e.preventManipulation();for(var s=0;s<e.changedTouches.length;++s){var o=e.changedTouches[s];if(n){i[o.identifier]=o.target}if(r){l(t,o,i[o.identifier],e)}else{a(t,o,i[o.identifier],e)}}};var h=function(e){return e.toLowerCase().replace("pointer","mouse")};var p=function(t,n,r){var i;if(r==r.toLowerCase()){var s=e.indexOf(r)-e.length/2;i=n+e[s]}else{i=n+r}if(i===n+"PointerEnter"&&t["on"+n.toLowerCase()+"pointerenter"]===undefined){i=n+"PointerOver"}if(i===n+"PointerLeave"&&t["on"+n.toLowerCase()+"pointerleave"]===undefined){i=n+"PointerOut"}return i};var d=function(e,t,n,r){if(e.__handjsRegisteredEvents===undefined){e.__handjsRegisteredEvents=[]}if(r){if(e.__handjsRegisteredEvents[t]!==undefined){e.__handjsRegisteredEvents[t]++;return}e.__handjsRegisteredEvents[t]=1;e.addEventListener(t,n,false)}else{if(e.__handjsRegisteredEvents.indexOf(t)!==-1){e.__handjsRegisteredEvents[t]--;if(e.__handjsRegisteredEvents[t]!=0){return}}e.removeEventListener(t,n);e.__handjsRegisteredEvents[t]=0}};var v=function(e,t,n){if(e.onpointerdown!==undefined){return}if(e.onmspointerdown!==undefined){var r=p(e,"MS",t);d(e,r,function(e){o(e,t)},n);return}if(e.ontouchstart!==undefined){switch(t){case"pointermove":d(e,"touchmove",function(e){c(e,t)},n);break;case"pointercancel":d(e,"touchcancel",function(e){c(e,t)},n);break;case"pointerdown":case"pointerup":case"pointerout":case"pointerover":case"pointerleave":case"pointerenter":if(!e.__handjsGlobalRegisteredEvents){e.__handjsGlobalRegisteredEvents=[]}if(n){if(e.__handjsGlobalRegisteredEvents[t]!==undefined){e.__handjsGlobalRegisteredEvents[t]++;return}e.__handjsGlobalRegisteredEvents[t]=1}else{if(e.__handjsGlobalRegisteredEvents[t]!==undefined){e.__handjsGlobalRegisteredEvents[t]--;if(e.__handjsGlobalRegisteredEvents[t]<0){e.__handjsGlobalRegisteredEvents[t]=0}}}break}}switch(t){case"pointerdown":d(e,"mousedown",function(e){u(e,t)},n);break;case"pointermove":d(e,"mousemove",function(e){u(e,t)},n);break;case"pointerup":d(e,"mouseup",function(e){u(e,t)},n);break;case"pointerover":d(e,"mouseover",function(e){u(e,t)},n);break;case"pointerout":d(e,"mouseout",function(e){u(e,t)},n);break;case"pointerenter":if(e.onmouseenter===undefined){d(e,"mouseover",function(e){u(e,t)},n)}else{d(e,"mouseenter",function(e){u(e,t)},n)}break;case"pointerleave":if(e.onmouseleave===undefined){d(e,"mouseout",function(e){u(e,t)},n)}else{d(e,"mouseleave",function(e){u(e,t)},n)}break}};var m=function(t){var n=t.prototype?t.prototype.addEventListener:t.addEventListener;var r=function(t,r,i){if(e.indexOf(t)!=-1){v(this,t,true)}if(n===undefined){this.attachEvent("on"+h(t),r)}else{n.call(this,t,r,i)}};if(t.prototype){t.prototype.addEventListener=r}else{t.addEventListener=r}};var g=function(t){var n=t.prototype?t.prototype.removeEventListener:t.removeEventListener;var r=function(t,r,i){if(e.indexOf(t)!=-1){v(this,t,false)}if(n===undefined){this.detachEvent(h(t),r)}else{n.call(this,t,r,i)}};if(t.prototype){t.prototype.removeEventListener=r}else{t.removeEventListener=r}};m(HTMLElement);m(document);m(HTMLBodyElement);m(HTMLDivElement);m(HTMLImageElement);m(HTMLUListElement);m(HTMLAnchorElement);m(HTMLLIElement);m(HTMLTableElement);if(window.HTMLSpanElement){m(HTMLSpanElement)}if(window.HTMLCanvasElement){m(HTMLCanvasElement)}if(window.SVGElement){m(SVGElement)}g(HTMLElement);g(document);g(HTMLBodyElement);g(HTMLDivElement);g(HTMLImageElement);g(HTMLUListElement);g(HTMLAnchorElement);g(HTMLLIElement);g(HTMLTableElement);if(window.HTMLSpanElement){g(HTMLSpanElement)}if(window.HTMLCanvasElement){g(HTMLCanvasElement)}if(window.SVGElement){g(SVGElement)}if(window.ontouchstart!==undefined){window.addEventListener("touchstart",function(e){for(var t=0;t<e.changedTouches.length;++t){var n=e.changedTouches[t];i[n.identifier]=n.target;l("pointerenter",n,n.target,e);l("pointerover",n,n.target,e);l("pointerdown",n,n.target,e)}});window.addEventListener("touchend",function(e){for(var t=0;t<e.changedTouches.length;++t){var n=e.changedTouches[t];var r=i[n.identifier];l("pointerup",n,r,e);l("pointerout",n,r,e);l("pointerleave",n,r,e)}});window.addEventListener("touchmove",function(e){for(var t=0;t<e.changedTouches.length;++t){var n=e.changedTouches[t];var r=document.elementFromPoint(n.clientX,n.clientY);var s=i[n.identifier];if(s===r){continue}if(s){l("pointerout",n,s,e);if(!s.contains(r)){l("pointerleave",n,s,e)}}if(r){l("pointerover",n,r,e);if(!r.contains(s)){l("pointerenter",n,r,e)}}i[n.identifier]=r}})}if(navigator.pointerEnabled===undefined){navigator.pointerEnabled=true;if(navigator.msPointerEnabled){navigator.maxTouchPoints=navigator.msMaxTouchPoints}}if(document.styleSheets&&document.addEventListener){document.addEventListener("DOMContentLoaded",function(){var e=function(e){return e.replace(/^\s+|\s+$/,"")};var t=function(t){var n=new RegExp(".+?{.*?}","m");var r=new RegExp(".+?{","m");while(t!=""){var i=n.exec(t);if(!i){break}var s=i[0];t=e(t.replace(s,""));var o=e(r.exec(s)[0].replace("{",""));if(s.replace(/\s/g,"").indexOf("touch-action:none")!=-1){var u=document.querySelectorAll(o);for(var a=0;a<u.length;a++){var f=u[a];if(f.style.msTouchAction!==undefined){f.style.msTouchAction="none"}else{f.handjs_forcePreventDefault=true}}}}};try{for(var n=0;n<document.styleSheets.length;n++){var r=document.styleSheets[n];if(r.href==undefined){continue}var i=new XMLHttpRequest;i.open("get",r.href,false);i.send();var s=i.responseText.replace(/(\n|\r)/g,"");t(s)}}catch(o){}var u=document.getElementsByTagName("style");for(var n=0;n<u.length;n++){var a=u[n];var f=e(a.innerHTML.replace(/(\n|\r)/g,""));t(f)}},false)}})()
  </script>
  <script>
  var Collection = function () {
    this.count = 0;
    this.collection = {};
    this.add = function (key, item) {
        if (this.collection[key] != undefined)
            return undefined;
        this.collection[key] = item;
        return ++this.count
    }
    this.remove = function (key) {
        if (this.collection[key] == undefined)
            return undefined;
        delete this.collection[key]
        return --this.count
    }
    this.item = function (key) {
        return this.collection[key];
    }
    this.forEach = function (block) {
        for (key in this.collection) {
            if (this.collection.hasOwnProperty(key)) {
                block(this.collection[key]);
            }
        }
    }
  }
  </script>
  
  <script>
  var Vector2 = function (x,y) {
  
  this.x= x || 0; 
  this.y = y || 0; 
  
};



Vector2.prototype = {

  reset: function ( x, y ) {

    this.x = x;
    this.y = y;

    return this;

  },

  toString : function (decPlaces) {
    decPlaces = decPlaces || 3; 
    var scalar = Math.pow(10,decPlaces); 
    return "[" + Math.round (this.x * scalar) / scalar + ", " + Math.round (this.y * scalar) / scalar + "]";
  },
  
  clone : function () {
    return new Vector2(this.x, this.y);
  },
  
  copyTo : function (v) {
    v.x = this.x;
    v.y = this.y;
  },
  
  copyFrom : function (v) {
    this.x = v.x;
    this.y = v.y;
  },  
  
  magnitude : function () {
    return Math.sqrt((this.x*this.x)+(this.y*this.y));
  },
  
  magnitudeSquared : function () {
    return (this.x*this.x)+(this.y*this.y);
  },
  
  normalise : function () {
    
    var m = this.magnitude();
        
    this.x = this.x/m;
    this.y = this.y/m;

    return this;  
  },
  
  reverse : function () {
    this.x = -this.x;
    this.y = -this.y;
    
    return this; 
  },
  
  plusEq : function (v) {
    this.x+=v.x;
    this.y+=v.y;
    
    return this; 
  },
  
  plusNew : function (v) {
     return new Vector2(this.x+v.x, this.y+v.y); 
  },
  
  minusEq : function (v) {
    this.x-=v.x;
    this.y-=v.y;
    
    return this; 
  },

  minusNew : function (v) {
    return new Vector2(this.x-v.x, this.y-v.y); 
  },  
  
  multiplyEq : function (scalar) {
    this.x*=scalar;
    this.y*=scalar;
    
    return this; 
  },
  
  multiplyNew : function (scalar) {
    var returnvec = this.clone();
    return returnvec.multiplyEq(scalar);
  },
  
  divideEq : function (scalar) {
    this.x/=scalar;
    this.y/=scalar;
    return this; 
  },
  
  divideNew : function (scalar) {
    var returnvec = this.clone();
    return returnvec.divideEq(scalar);
  },

  dot : function (v) {
    return (this.x * v.x) + (this.y * v.y) ;
  },
  
  angle : function (useRadians) {
    
    return Math.atan2(this.y,this.x) * (useRadians ? 1 : Vector2Const.TO_DEGREES);
    
  },
  
  rotate : function (angle, useRadians) {
    
    var cosRY = Math.cos(angle * (useRadians ? 1 : Vector2Const.TO_RADIANS));
    var sinRY = Math.sin(angle * (useRadians ? 1 : Vector2Const.TO_RADIANS));
  
    Vector2Const.temp.copyFrom(this); 

    this.x= (Vector2Const.temp.x*cosRY)-(Vector2Const.temp.y*sinRY);
    this.y= (Vector2Const.temp.x*sinRY)+(Vector2Const.temp.y*cosRY);
    
    return this; 
  },  
    
  equals : function (v) {
    return((this.x==v.x)&&(this.y==v.x));
  },
  
  isCloseTo : function (v, tolerance) { 
    if(this.equals(v)) return true;
    
    Vector2Const.temp.copyFrom(this); 
    Vector2Const.temp.minusEq(v); 
    
    return(Vector2Const.temp.magnitudeSquared() < tolerance*tolerance);
  },
  
  rotateAroundPoint : function (point, angle, useRadians) {
    Vector2Const.temp.copyFrom(this); 
    //trace("rotate around point "+t+" "+point+" " +angle);
    Vector2Const.temp.minusEq(point);
    //trace("after subtract "+t);
    Vector2Const.temp.rotate(angle, useRadians);
    //trace("after rotate "+t);
    Vector2Const.temp.plusEq(point);
    //trace("after add "+t);
    this.copyFrom(Vector2Const.temp);
    
  }, 
  
  isMagLessThan : function (distance) {
    return(this.magnitudeSquared()<distance*distance);
  },
  
  isMagGreaterThan : function (distance) {
    return(this.magnitudeSquared()>distance*distance);
  }
  
  
  // still AS3 to convert : 
  // public function projectOnto(v:Vector2) : Vector2
  // {
  //    var dp:Number = dot(v);
  // 
  //    var f:Number = dp / ( v.x*v.x + v.y*v.y );
  // 
  //    return new Vector2( f*v.x , f*v.y);
  //  }
  // 
  // 
  // public function convertToNormal():void
  // {
  //  var tempx:Number = x; 
  //  x = -y; 
  //  y = tempx; 
  //  
  //  
  // }    
  // public function getNormal():Vector2
  // {
  //  
  //  return new Vector2(-y,x); 
  //  
  // }
  // 
  // 
  // 
  // public function getClosestPointOnLine ( vectorposition : Point, targetpoint : Point ) : Point
  // {
  //  var m1 : Number = y / x ;
  //  var m2 : Number = x / -y ;
  //  
  //  var b1 : Number = vectorposition.y - ( m1 * vectorposition.x ) ;
  //  var b2 : Number = targetpoint.y - ( m2 * targetpoint.x ) ;
  //  
  //  var cx : Number = ( b2 - b1 ) / ( m1 - m2 ) ;
  //  var cy : Number = m1 * cx + b1 ;
  //  
  //  return new Point ( cx, cy ) ;
  // }
  // 

};

Vector2Const = {
  TO_DEGREES : 180 / Math.PI,   
  TO_RADIANS : Math.PI / 180,
  temp : new Vector2()
  };
  </script>
  
  <style>
    * {
    -webkit-touch-callout: none; /* prevent callout to copy image, etc when tap to hold */
    -webkit-text-size-adjust: none; /* prevent webkit from resizing text to fit */
    /* make transparent link selection, adjust last value opacity 0 to 1.0 */
    -webkit-tap-highlight-color: rgba(0,0,0,0);
    -webkit-user-select: none; /* prevent copy paste, to allow, change 'none' to 'text' */
    -webkit-tap-highlight-color: rgba(0,0,0,0);
    touch-action: none;
    }

    body {
      background-color: #000000;
    overflow:hidden;
      margin: 0px;
    }

    canvas {
    
      background-color: #111133;
      display: block;
      position: absolute;
    }
  
  hr{
        height: 1px;
        background-color: black;
        border: none;
  }

    .container {
      width: auto;
      text-align: center;
      background-color: #ff0000;
    }
  
  .footer{
    background:orange;
    position: fixed;
    left: 0;
      bottom: 0;
    width:100%%;
    height:20%%;
    text-align: center;
  }
  
  .infos {
    -ms-box-orient: horizontal;
    display: -webkit-box;
    display: -moz-box;
    display: -ms-flexbox;
    display: -moz-flex;
    display: -webkit-flex;
    display: flex;
    flex-direction: row;
    align-items: center;
    justify-content: center;
    height:100%%;
   }
   
   .leftFooter, .rightFooter, .middleFooter {
    height:100%%;
    flex: 1 1 30%%; /*grow | shrink | basis */
    border: 1px solid;
   }
  
  </style>
</head>
<body>
    <div class="container">
        <canvas id="canvasSurfaceGame"></canvas>
        
    </div>
  <footer class="footer">
    <div class="infos">
      <div class="leftFooter">
        <p class="lap">lap: <span id="lap">%LAP%</span> / checkpoint: <span id="checkpoint">%CHECKPOINT%</span></p>
      </div>
      <div class="middleFooter">
        <p>© LEGOKart</p>
      </div>
      <div class="rightFooter"> 
        <p class="rfid">Classement : <span id="rfid">%RANK%</span> </p>
      </div>
    </div>
    </footer>
</body>

<script>
var gateway = `ws://${window.location.hostname}/ws`;
var websocket;
var isstarted=false;
var x=0;
var y=0;
var speed1=0.0;
var speed2=0.0;
var packetcounter=0;
var speedmodified=false;
var objet;
var aObjet=false;
var myImage=new Image();

var myleftnewx=0.0;
var myleftnewy=0.0;
var mynewx=0.0;
var mynewy=0.0;
window.addEventListener('load', onLoad);

window.requestAnimFrame = (function () {
    return window.requestAnimationFrame ||
    window.webkitRequestAnimationFrame ||
    window.mozRequestAnimationFrame ||
    window.oRequestAnimationFrame ||
    window.msRequestAnimationFrame ||
    function (callback) {
        window.setTimeout(callback, 1000 / 60);
    };
})();

var canvas,
c, // c is the canvas' context 2D
container,
thirdWidth,
thirdHeight,
twothirdWidth,
twothirdHeight,
leftPointerID = -1,
leftPointerPos = new Vector2(0, 0),
leftPointerStartPos = new Vector2(0, 0),
leftVector = new Vector2(0, 0),
rightPointerID = -1,
rightPointerPos = new Vector2(0, 0),
rightPointerStartPos = new Vector2(0, 0),
rightVector = new Vector2(0, 0),
middlePointerID=-1;

const rayon=50;
const packetlimit=8;

var touches; // collections of pointers
var ship;

document.addEventListener("DOMContentLoaded", init);

window.onorientationchange = resetCanvas;
window.onresize = resetCanvas;

function init() {
    setupCanvas();
    touches = new Collection();
    canvas.addEventListener('pointerdown', onPointerDown, false);
    canvas.addEventListener('pointermove', onPointerMove, false);
    canvas.addEventListener('pointerup', onPointerUp, false);
    canvas.addEventListener('pointerout', onPointerUp, false);
    
    myImage.onload=function(){
      c.drawImage(myImage,thirdWidth,0,thirdWidth,thirdWidth);
    };
    myImage.src="https://www.nicepng.com/png/full/154-1548752_image-result-for-item-box-mario-kart-wii.png";
    requestAnimFrame(draw);
}

function resetCanvas(e) {
    // resize the canvas - but remember - this clears the canvas too. 
    canvas.width = window.innerWidth;
    canvas.height = window.innerHeight*0.8;

  thirdWidth= canvas.width/3;
  thirdHeight= canvas.height/3;
  twothirdWidth= (2*canvas.width)/3;
  twothirdHeight= (2*canvas.width)/3;

    //make sure we scroll to the top left. 
    window.scrollTo(0, 0);
}


function draw() {
    c.clearRect(0, 0, canvas.width, canvas.height);
    c.drawImage(myImage,thirdWidth,0,thirdWidth,thirdWidth);
    touches.forEach(function (touch) {
        if (touch.identifier == leftPointerID) {
            c.beginPath();
            c.strokeStyle = "cyan";
            c.lineWidth = 6;
      
            c.arc(leftPointerStartPos.x, leftPointerStartPos.y, 40, 0, Math.PI * 2, true);
            c.stroke();
            c.beginPath();
            c.strokeStyle = "cyan";
            c.lineWidth = 2;
            c.arc(leftPointerStartPos.x, leftPointerStartPos.y, 60, 0, Math.PI * 2, true);
            c.stroke();
      
      c.beginPath();
            c.strokeStyle = "cyan";
            c.arc(leftPointerPos.x, leftPointerPos.y, 40, 0, Math.PI * 2, true);
            c.stroke();

        }         
    if (touch.identifier == rightPointerID) {
            c.beginPath();
            c.strokeStyle = "cyan";
            c.lineWidth = 6;
      
            c.arc(rightPointerStartPos.x, rightPointerStartPos.y, 40, 0, Math.PI * 2, true);
            c.stroke();
            c.beginPath();
            c.strokeStyle = "cyan";
            c.lineWidth = 2;
            c.arc(rightPointerStartPos.x, rightPointerStartPos.y, 60, 0, Math.PI * 2, true);
            c.stroke();
      
      c.beginPath();
            c.strokeStyle = "cyan";
            c.arc(rightPointerPos.x, rightPointerPos.y, 40, 0, Math.PI * 2, true);
            c.stroke();

        }
    if (touch.identifier == middlePointerID) {
            c.beginPath();
            c.strokeStyle = "red";
            c.lineWidth = "6";
            c.arc(touch.x, touch.y, 40, 0, Math.PI * 2, true);
            c.stroke();
        }
    else {
      //si on a envie
        }
    });
  
  c.beginPath();
    c.strokeStyle = "white";
  c.lineWidth = "6";
  c.moveTo(thirdWidth,0);
  c.lineTo(thirdWidth, canvas.height);
  c.stroke();
  
  c.beginPath();
    c.strokeStyle = "white";
  c.lineWidth = "6";
  c.moveTo(twothirdWidth,0);
  c.lineTo(twothirdWidth, canvas.height);
  c.stroke();

    
  if (isstarted && packetcounter>=packetlimit && speedmodified){
    //console.log(x+"-"+y);
    
    websocket.send("SPEED:"+speed1+":"+speed2);
    date = Date.now();
    packetcounter=0
  }
  packetcounter++;
  
  requestAnimFrame(draw);
}


function givePointerType(event) {
    switch (event.pointerType) {
        case event.POINTER_TYPE_MOUSE:
            return "MOUSE";
            break;
        case event.POINTER_TYPE_PEN:
            return "PEN";
            break;
        case event.POINTER_TYPE_TOUCH:
            return "TOUCH";
            break;
    }
}

function onPointerDown(e) {
    var newPointer = { identifier: e.pointerId, x: e.clientX, y: e.clientY, type: givePointerType(e) };
    if ((leftPointerID < 0) && (e.clientX < thirdWidth)) {
        leftPointerID = e.pointerId;
        leftPointerStartPos.reset(e.clientX, e.clientY);
        leftPointerPos.copyFrom(leftPointerStartPos);
        leftVector.reset(0, 0);
    }
    if ((rightPointerID < 0) && (e.clientX > twothirdWidth)) {
        speedmodified=true;
        rightPointerID = e.pointerId;
        rightPointerStartPos.reset(e.clientX, e.clientY);
        rightPointerPos.copyFrom(rightPointerStartPos);
        rightVector.reset(0, 0);
        
    }
  if ((middlePointerID < 0) && (e.clientX < twothirdWidth) && (e.clientX > thirdWidth)) {
        middlePointerID = e.pointerId;
        if(aObjet ==true){
          myImage.src="https://www.nicepng.com/png/full/154-1548752_image-result-for-item-box-mario-kart-wii.png";
          websocket.send(objet);
          //je ne comprends pas pourquoi mais ça fait redémarrer le M5Stack
          //il faut aussi check le bug qui fait que ça passe toujours dans le if
          aObjet =false;
        }
        
    //fonction d'appel websocket objet
    }
    touches.add(e.pointerId, newPointer);
    packetcounter=packetlimit;
}

function onPointerMove(e) {
    if (leftPointerID == e.pointerId) {
    myleftnewx = e.clientX;
    myleftnewy = e.clientY;
    dx =(parseInt(leftPointerStartPos.x)-parseInt(e.clientX));
    dy =(parseInt(leftPointerStartPos.y)-parseInt(e.clientY));
    if (Math.sqrt(Math.pow((dx),2) + Math.pow((dy),2))>rayon){
      theta = Math.atan2(dy, dx);
      myleftnewx = (leftPointerStartPos.x - rayon*Math.cos(theta));
      myleftnewy = (leftPointerStartPos.y - rayon*Math.sin(theta));
    }
        leftPointerPos.reset(myleftnewx, myleftnewy);
        leftVector.copyFrom(leftPointerPos);
        leftVector.minusEq(leftPointerStartPos);
        x = parseFloat(parseInt(leftPointerStartPos.x)-parseInt(myleftnewx))/(75.0);
    }
  if (rightPointerID == e.pointerId) {
    mynewx = e.clientX;
    mynewy = e.clientY;
    dx =(parseInt(rightPointerStartPos.x)-parseInt(e.clientX));
    dy =(parseInt(rightPointerStartPos.y)-parseInt(e.clientY));
    if (Math.sqrt(Math.pow((dx),2) + Math.pow((dy),2))>rayon){
      theta = Math.atan2(dy, dx);
      mynewx = (rightPointerStartPos.x - rayon*Math.cos(theta));
      mynewy = (rightPointerStartPos.y - rayon*Math.sin(theta));
    }
        rightPointerPos.reset(mynewx, mynewy);
        rightVector.copyFrom(rightPointerPos);
        rightVector.minusEq(rightPointerStartPos);
        x = parseFloat(parseInt(leftPointerStartPos.x)-parseInt(myleftnewx))/(rayon*1.3);
        y = parseFloat(parseInt(rightPointerStartPos.y)-parseInt(mynewy))/(rayon);
    }
  if(x<0){
    speed1=parseFloat(((1.0)+x)*y);
    speed2=parseFloat(y);
  } else if (x>=0){
    speed1=parseFloat(y);
    speed2=parseFloat(((1.0)-x)*y);
  }

} 

function onPointerUp(e) {
    if (leftPointerID == e.pointerId) {
        leftVector.reset(0, 0);
        speed1=parseFloat(y);
        speed2=parseFloat(y);
        leftPointerStartPos.x=0;
        myleftnewx=0;
        mynewy=0;
        leftPointerID = -1;
        
  
    }
  if (rightPointerID == e.pointerId) {
        rightVector.reset(0, 0);
        speedmodified=false;
        x=0;
        y=0;
        myleftnewx=0;
        mynewy=0;
        speed1=0;
        speed2=0;
        websocket.send("SPEED:"+speed1+":"+speed2);
        rightPointerID = -1;

    }
  if (middlePointerID == e.pointerId) {
    
        middlePointerID = -1;
        middleVector.reset(0, 0);
    }
    touches.remove(e.pointerId);
  packetcounter=packetlimit;
}

function setupCanvas() {
    canvas = document.getElementById('canvasSurfaceGame');
    c = canvas.getContext('2d');
    resetCanvas();
    c.strokeStyle = "#ffffff";
    c.lineWidth = 2;
}



  function initWebSocket() {
    console.log('Trying to open a WebSocket connection...');
    websocket = new WebSocket(gateway);
    websocket.onopen    = onOpen;
    websocket.onclose   = onClose;
    websocket.onmessage = onMessage; // <-- add this line
  }
  function onOpen(event) {
    console.log('Connection opened');
    isstarted=true;
  }
  function onClose(event) {
    console.log('Connection closed');
    setTimeout(initWebSocket, 2000);
  }
  
  function onLoad(event) {
    initWebSocket();
    initButton();
  }
  
  function onMessage(event) {
    if(event.data=="carapaceVerte"){
      objet ="cv";
      aObjet=true;
      myImage.src="https://www.nicepng.com/png/full/867-8676588_mario-kart-turtle-shells-super-mario-turtle-shell.png";
      
    }else {
      if(event.data=="carapaceRouge"){
        objet ="cr";
        aObjet=true;
        myImage.src="https://www.nicepng.com/png/full/867-8676747_mario-kart-green-shells-red-shell.png";
      }else{
        var myMessageContent = event.data.split(':');
        if(myMessageContent[0]=="Classement"){
          document.getElementById('rfid').innerHTML = myMessageContent[1];
        }else{
          if(myMessageContent[0]=="Checkpoint"){
            document.getElementById('lap').innerHTML = Math.floor(parseInt(myMessageContent[1])/3)+1;
            document.getElementById('checkpoint').innerHTML = parseInt(myMessageContent[1])%3;
          }
        }
      }
    }
    
  }
  
</script>
</html>
)rawliteral";

const char BROADSYNC[] PROGMEM = R"rawliteral(
<html lang="fr">
<head>
    <meta charset="utf-8">
    <meta name="viewport" content="width=device-width; height=device-height; initial-scale=1.0; maximum-scale=1.0;" />
    <title>Page to get IP</title>
</head>
<body>
%IPSTACK%
</body>
</html>
)rawliteral";

char** str_split(char* a_str, const char a_delim)
{
    char** result    = 0;
    size_t count     = 0;
    char* tmp        = a_str;
    char* last_comma = 0;
    char delim[2];
    delim[0] = a_delim;
    delim[1] = 0;

    /* Count how many elements will be extracted. */
    while (*tmp)
    {
        if (a_delim == *tmp)
        {
            count++;
            last_comma = tmp;
        }
        tmp++;
    }

    /* Add space for trailing token. */
    count += last_comma < (a_str + strlen(a_str) - 1);

    /* Add space for terminating null string so caller
       knows where the list of returned strings ends. */
    count++;

    result = (char**)malloc(sizeof(char*) * count);

    if (result)
    {
        size_t idx  = 0;
        char* token = strtok(a_str, delim);

        while (token)
        {
            assert(idx < count);
            *(result + idx++) = strdup(token);
            token = strtok(0, delim);
        }
        assert(idx == count - 1);
        *(result + idx) = 0;
    }

    return result;
}

void notifyClients() {
  ws.textAll((String)LapNB+"-"+(String)checkPointNB+"-"+ RFIDvalue);
}



// sert pour le premier affichage de la page (peut etre supprimable ?)
String processor(const String& var){
  if(var == "SPEED"){
    return (const __FlashStringHelper*)((speed1)+"-"+(speed2));
  }
  if (var =="RANK"){
    return (String(getRank()+1));
  }
  if (var =="LAP"){
    return (String(NbCP/3));
  }
  if (var =="CHECKPOINT"){
    return (String(NbCP%3));
  }
  
} 

void updateClassement(){
  ws.textAll(String("Classement:")+String(getRank()+1));
}
void updateCheckpoint(){
  ws.textAll(String("Checkpoint:")+String(NbCP));
}

 int maxofarray(int a[],int n)
 {
  int maxi,i;
  maxi=a[0];
  int indice = 0;
    for(i=1; i<n; i++)
    {   
       if(maxi<a[i]){
        maxi=a[i]; 
        indice=i;    
       }  
    }
    
    return indice;
 }

  int minofarray2(long a[][2],int n)
 {
  int mini,i;
  mini=a[0][1];
  int indice = 0;
    for(i=1; i<n; i++)
    {   
       if(mini>a[i][1]){
        mini=a[i][1]; 
        indice=i;      
       }
    }
    
    return indice;
 }

void classement2(){
    Serial.println("entrée de classement 2");
    Serial.println(nbJoueurs);
  int tab_inter[N];
  for (int i=0;i<nbJoueurs;i++){
    tab_inter[i] = -1;
    for (int j=nbCheckpoints-1;j>=0;j--){
      Serial.println(String(j)+" "+String(etat_course[i][j])+ " " +String(i));
      if (etat_course[i][j]!=0){
        tab_inter[i] = j;
        break;
        
      }
      
    }
  }

  int nbranked = 0;
  Serial.println("mid calssemene");
  
  for (int h=0;h<nbJoueurs;h++){
    int counter = 0;
    int indice_max = maxofarray(tab_inter,nbJoueurs);
    Serial.println("indice max" + String(indice_max) + " for round " + String(h));
    int val_max = tab_inter[indice_max];
    Serial.println("Valeur Max"+String(val_max));
    long same_pos[nbJoueurs][2];
    for (int a=0;a<nbJoueurs;a++){
      same_pos[a][0] = 1000;
      same_pos[a][1] = 2147483646;
      if (tab_inter[a] == val_max && tab_inter[a] >=-1){
        same_pos[counter][0] = (long)a;
        Serial.println(a);
        same_pos[counter][1] = (long)etat_course[a][val_max];
        Serial.println(etat_course[a][val_max]);
        counter+=1;
        tab_inter[a] = -2;
      }
    }
    Serial.println(counter);
    for (int b=0;b<counter;b++){
      int indice_min = minofarray2(same_pos,nbJoueurs);
      Serial.println("indice min" + String(indice_min) + " for round " + String(h));
      rank[nbranked] = IPs_Liste[same_pos[indice_min][0]];
      Serial.print(IPs_Liste[same_pos[indice_min][0]]);
      Serial.print(" || ");
      Serial.print(same_pos[indice_min][0]);
      Serial.print(" || ");
      Serial.println(nbranked);
      nbranked++;
      same_pos[indice_min][1]=2147483646;
    }
  }
  updateClassement();
}


void resetWifi()
{
  Serial.println("Suppression des reglages et redemarrage ...");
   // Reset des paramètres Wifi
   wm.resetSettings();
  //WiFi.mode(WIFI_OFF);
  delay(1000);
  ESP.restart();
}

void forward(int speedOne,int speedTwo){
  if(paralized){
    Serial.println("je tourne ............ je me raccroche à la vie");
    goPlus.Motor_write_speed(MOTOR_NUM0, 127); // il va tourner en rond car un des deux moteurs est inversé
    goPlus.Motor_write_speed(MOTOR_NUM1, 127);
  }else{
    goPlus.Motor_write_speed(MOTOR_NUM0, speedOne); // valeur inversé moteur num0 : moteur droit
    goPlus.Motor_write_speed(MOTOR_NUM1, -speedTwo*0.95); // moteur gauche
  }
  
}



void carapaceVerte(){
  Serial.println("Carapace Verte");
  if(random(0, 100)>probaCarapaceVerte){
    paralizedAt = millis();
    paralized = true;
    Serial.println("touche verte");
  }
}
void carapaceRouge(){
  Serial.println("Carapace Rouge");
  paralizedAt = millis();
  paralized = true;
}

void sendUdpPlus(String donnee,IPAddress ipAdd){
  String msg = donnee;
  msg += String(";");
  msg += String(sequence);
  Serial.println(ipAdd);
  Serial.println("Envoie du paquet udp ........");
  Serial.println(msg);
  udp.writeTo((uint8_t*)msg.c_str(),msg.length()+1,ipAdd,(uint16_t)42424);
  Serial.println("Paquet udp envoyé");
  Message m;
  Serial.println("Avant les changement de variable");
  m.ack = sequence;
  m.donnee = donnee;
  m.nbEnvoi = 1;
  m.ip = ipAdd;
  m.myTime =millis();
  Serial.println("on met l'objet dans le tableau");
  messages[messagesSize]=m;
  Serial.println("update de sequence");
  sequence = (sequence)%(maxqueue-1)+1;                     //c'est pour ne pas avoir de ACK = 0 comme ça si un toInt renvoie 0 c'est qu'il est sur un String
  Serial.println("incrementation d'une variable");
  messagesSize++;
  Serial.println("Je sors de SendUdpPlus");
}


void checkUdpRtt(){
  
  for(int i=0;i<messagesSize;i++){
    if(millis()-messages[i].myTime>rtt){
      Serial.println("Je suis dans checkUdpRtt et je vais renvoyer un message");
      String msg = messages[i].donnee;
      msg += String(";");
      msg += String(messages[i].ack);
      Serial.println(messages[i].ip);
      udp.writeTo((uint8_t*)msg.c_str(),msg.length()+1,messages[i].ip,(uint16_t)42424);
      Serial.println("Je suis dans checkUdpRtt et message envoyé");
      messages[i].nbEnvoi+=1;
      if(messages[i].nbEnvoi>=maxRepetition){
        messages[i]=messages[messagesSize-1];
        messagesSize--;
      }
      messages[i].myTime=millis();
      Serial.println("Fin du checkudp");
    }
  }
  
}
void checkParalized(){
  if(paralized && millis()-paralizedAt>tempsParalized){
    paralized =false;
  }
}

void handleWebSocketMessage(void *arg, uint8_t *data, size_t len) {
  AwsFrameInfo *info = (AwsFrameInfo*)arg;
  if (info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT) {
    data[len] = 0;
    
    if(strcmp((char *)data,"cv") == 0 ||strcmp((char *)data,"cr") == 0){
      IPAddress ip = WiFi.localIP();
      ip[3] = Find_Pred();
      Serial.println("Je rentre dans SendUDPplus");
      sendUdpPlus(String((char *)data),ip);
      Serial.println("Je suis sorti de SendUdpPlus");
    }
    char **donnee = str_split((char *)data,':');
    if (strcmp(donnee[0],"SPEED") == 0) {
      speed1=MAXSPEED*(strtof(donnee[1],NULL));
      speed2=MAXSPEED*(strtof(donnee[2],NULL));
      notifyClients();
    }
  }
}


void onEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type,
             void *arg, uint8_t *data, size_t len) {
  switch (type) {
    case WS_EVT_CONNECT:
      break;
    case WS_EVT_DISCONNECT:
      break;
    case WS_EVT_DATA:
      handleWebSocketMessage(arg, data, len);
      break;
    case WS_EVT_PONG:
    case WS_EVT_ERROR:
      break;
  }
}



void initWebSocket() {
  ws.onEvent(onEvent);
  server.addHandler(&ws);
}

void setup() {
  boolean bistable=false;
  M5.begin();
  Serial.begin(115200);
  goPlus.begin();
  mfrc522.PCD_Init();
  delay(100);
  for(int i=0;i<N;i++){
    IPs_Liste[i] = -1;
  }


  M5.Lcd.begin();
  
  Wire.begin();// Initialize I2C
  for(int k=0;k<N;k++){
    for(int l=0;l<100;l++){
      etat_course[k][l]=0;// on initialise le tableau à 0
    }
  }


  M5.Lcd.qrcode("WIFI:S:LUIGI_WIFI;T:WPA;P:mariokartmindstorm;;",0,0,220,6);
   // Connect to Wi-Fi
  wm.setWiFiAPChannel(8);
  wm.setConnectRetries(5);
  wm.setConnectTimeout(3);
  WiFi.mode(WIFI_STA);  // Définition du Wifi en mode Station
  delay(1000);
  Serial.println("\n");

  
  if (!wm.getWiFiIsSaved()){
    bistable = true;
  }
  if(!wm.autoConnect(ssid, password)){  // Test d'auto-connexion
    Serial.println("Erreur de connexion.");  // Si pas de connexion = Erreur
  }
  else{
    // Si connexion = OK
      Serial.println("Connexion etablie !");
      M5.Lcd.qrcode("http://"+String(WiFi.localIP())+"/game/",0,0,220,6);
      M5.Lcd.println(IP_Courante);
  }
  if (bistable == true && WiFi.isConnected()){
      Serial.println("ouais ça rentre");
      ESP.restart();
  }
  
  IP_Courante = WiFi.localIP();
  IPs_Liste[0] = IP_Courante[3];
  rank[0] = IP_Courante[3];

  
  
  timeClient.begin();
  sender = "Are you here Mario?";
  receiver = "Its Me Mario!";
  if(udp.listen(42424)) {
      udp.onPacket([](AsyncUDPPacket packet) {
          Serial.println("donnee du packet : ");
          String maData = String((char *)packet.data());
          if (maData == String(sender)){
             String msg = "Its Me Mario!";
             udp.writeTo((uint8_t*)msg.c_str(),msg.length()+1,packet.remoteIP(),(uint16_t)42424);
             
             for (int i=0;i<N;i++){
              if(IPs_Liste[i]== -1){
                IPs_Liste[i] =packet.remoteIP()[3];
                nbJoueurs++;
                break;
              }
              if(IPs_Liste[i] == packet.remoteIP()[3]){
                break;
              }
              if (IPs_Liste[i] > packet.remoteIP()[3]){
                for (int j=N-2;j>=i;j--){
                  IPs_Liste[j+1] = IPs_Liste[j];
                }
                IPs_Liste[i] =packet.remoteIP()[3];
                nbJoueurs++;
                break;
              }
            }
            for(int i =0;i<N;i++){
              rank[i]=IPs_Liste[i];
            }
            Serial.println(" Ma liste d'IP");
            for(int i=0;i<nbJoueurs;i++){
              Serial.print(" |");
              Serial.print(IPs_Liste[i]);
              Serial.print("| ");
            }
            Serial.println("");
            
          } else if (maData == String(receiver)){

             for (int i=0;i<N;i++){
              if(IPs_Liste[i]== -1){
                IPs_Liste[i] =packet.remoteIP()[3];
                nbJoueurs++;
                break;
              }
              if(IPs_Liste[i] == packet.remoteIP()[3]){
                break;
              }
              if (IPs_Liste[i] > packet.remoteIP()[3]){
                for (int j=N-2;j>=i;j--){
                  IPs_Liste[j+1] = IPs_Liste[j];
                }
                IPs_Liste[i] =packet.remoteIP()[3];
                nbJoueurs++;
                break;
              }
            }
            for(int i =0;i<N;i++){
              rank[i]=IPs_Liste[i];
            }
           
            Serial.println(" Ma liste d'IP");
            for(int i=0;i<nbJoueurs;i++){
              Serial.print(" |");
              Serial.print(IPs_Liste[i]);
              Serial.print("| ");
            }
            Serial.println("");
          } else {
            //***********************************************
            int index = maData.indexOf(";");
            String partOne = maData.substring(0,index);
            String partTwo = maData.substring(index+1);
            Serial.println("partOne = ");
            Serial.println(partOne);
            Serial.println("partTwo = ");
            Serial.println(partTwo);
            if(atoi(partOne.c_str())!=0){
              for(int i=0;i<messagesSize;i++){                              //pour check si c'est un ACK d'un message qu'on a envoyé avant
                if(messages[i].ack == atoi(partOne.c_str())){                  //toInt() retourne 0 si c'est un pas convertible
                  Serial.println("J'ai recu un ACK");
                  //free(messages[i]);
                  messages[i]=messages[messagesSize-1];
                  messagesSize--;
                  i= messagesSize;                                          //manière un peu sale de finir le for quand on a trouvé;    
                }
              }
            }else if(atoi(partTwo.c_str())!=0){
              String msg = String(partTwo);
              Serial.println("Je vais envoyé l'ack");
              udp.writeTo((uint8_t*)msg.c_str(),msg.length()+1,packet.remoteIP(),(uint16_t)42424);  //on lui retourne son ACK
              Serial.println("J'ai envoyé l'ack");
              if(partOne == String(messCarapaceVerte)){
                Serial.println("J'ai recu une carapace verte");
                carapaceVerte();
              }if(partOne == String(messCarapaceRouge)){
                Serial.println("J'ai recu une carapace rouge");
                carapaceRouge();
              }
              int index2 = partOne.indexOf(":");
              int index3 = partOne.indexOf(":",index2+1);
              String partOneOne = partOne.substring(0,index2); //"cp"
              String partOneTwo = partOne.substring(index2+1,index3); //NbCP
              String partOneThree = partOne.substring(index3+1); //Time
              Serial.println("partOne : ");
              Serial.println(partOne);
              Serial.println("partTwo : ");
              Serial.println(partTwo);
              Serial.println("partOneOne : ");
              Serial.println(partOneOne);
              Serial.println("partOneTwo : ");
              Serial.println(partOneTwo);
              Serial.println("partOneThree : ");
              Serial.println(partOneThree);
              
              
             if(partOneOne == String(messCheckPoint)){
                Serial.println("Je rentre dans le truc");
                long value=atol(partOneThree.c_str());
                unsigned long  temps=(unsigned long)value; 
                int NbCP2=atoi(partOneTwo.c_str());
                
                for(int i=0;i<N;i++){
                  if(IPs_Liste[i]==packet.remoteIP()[3]){
                      etat_course[i][NbCP2-1]= temps;
                  }
                }
                classement2(); 
                
              }
              }
                
            }
            //***********************************************                  
           }
    );
  }
  udp.broadcastTo("Are you here Mario?", 42424);
  Serial.println(IP_Courante);
  WiFi.begin();

  initWebSocket();
  // Route for root / web page
  server.on("/game/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html, processor);
    updateCheckpoint();
  });

  //init des tags lus
  lastRFID=String("FFFFFFFFFFFFFFFFFFFF");
  RFIDvalue=String("FFFFFFFFFFFFFFFFFFFF");
  // Start server
  server.begin();

}


void loop() {
  M5.update();
  if (M5.BtnB.isPressed()){
    resetWifi();
  }
  forward(speed1,speed2);
  checkUdpRtt();
  checkParalized();
  if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial() ) {
      timeClient.update();
      unsigned long temps_lecture_tag=timeClient.getEpochTime();
      lastRFID=String("");
    for (int l=0;l<mfrc522.uid.size;l++){
      lastRFID += String(mfrc522.uid.uidByte[l], HEX);
    }
    
    if (!RFIDvalue.equals(lastRFID)) {
      Serial.println("actualisation de RFID Value");
      Serial.println( lastRFID);
      RFIDvalue=lastRFID;
      
      if(Contains_Tab(objets_ids,(char*)RFIDvalue.c_str(),4)){
        ws.textAll(objets[random(0,nbObjets)]);
      }
      
      if(Contains_Tab(ids_CP1,(char*)RFIDvalue.c_str(),7) && NbCP%3==0){  
         NbCP++;
         updateCheckpoint();
         Serial.println("Vous etes passe sur le premier checkpoint!");
         chaine_a_envoyer=String("cp:")+String(NbCP)+String(":")+String(temps_lecture_tag);   //je ne suis pas sur du %u mais il me parait le plus adapté
         Serial.println("Chaine a envoyer : --------------------");
         Serial.println(chaine_a_envoyer);
         for(int k=0;k<nbJoueurs;k++){ //send(message) aux autres karts comme quoi le kart courant a dépassé le checkpoint 1 
          //il faut trouver un moyen pour pas que le kart courant s'envoie pas le msg à lui-même
          Serial.println("je rentre dans le for ligne 1685");
          IPAddress ip_a_envoyer=IP_Courante;
          ip_a_envoyer[3]=IPs_Liste[k];
          if(IPs_Liste[k] != IP_Courante[3]){
            sendUdpPlus(chaine_a_envoyer,ip_a_envoyer);
          }
      
      
         }
        for(int i=0;i<N;i++){
      
          if(IPs_Liste[i]==IP_Courante[3]){
            etat_course[i][NbCP-1]= temps_lecture_tag ;
          }
        } //etat_course[i] =[temps1,temps2,temps3,...] temps1 ->NbCP=1(indice 0 du tableau) ,temps2->NbCP=2 (indice 1 du tableau) , pour avoir acces au temps associé au  NbCP=j pour le joueur 1, on prends la case [joeur1][j-1] 
        classement2();
      }
      if(Contains_Tab(ids_CP2,(char*)RFIDvalue.c_str(),7) && NbCP%3==1){
      NbCP++;
      updateCheckpoint();
      Serial.println("Vous etes passe sur le deuxieme checkpoint!");
      chaine_a_envoyer="cp:"+String(NbCP)+":"+String(temps_lecture_tag);
      for(int k=0;k<N;k++){ //send(message) aux autres karts comme quoi le kart courant a dépassé le checkpoint 2 
        IPAddress ip_a_envoyer=IP_Courante;
        ip_a_envoyer[3]=IPs_Liste[k];
      
      if(IPs_Liste[k] != IP_Courante[3]){
         sendUdpPlus(chaine_a_envoyer,ip_a_envoyer);
      }
     }  
      for(int i=0;i<N;i++){
      
        if(IPs_Liste[i]==IP_Courante[3]){
          etat_course[i][NbCP-1]= temps_lecture_tag ;
        }
      }
      classement2();
    }
    if(Contains_Tab(ids_CP3,(char*)RFIDvalue.c_str(),7) && NbCP%3==2){
      NbCP++;
      updateCheckpoint();
      Serial.println("Vous etes passe sur le troisieme checkpoint!");
      chaine_a_envoyer="cp:"+String(NbCP)+":"+String(temps_lecture_tag);  
      for(int k=0;k<N;k++){ //send(message) aux autres karts comme quoi le kart courant a dépassé le checkpoint 3 
        IPAddress ip_a_envoyer=IP_Courante;
        ip_a_envoyer[3]=IPs_Liste[k];
      
        if(IPs_Liste[k] != IP_Courante[3]){
          sendUdpPlus(chaine_a_envoyer,ip_a_envoyer);
        }      
      }
    for(int i=0;i<N;i++){
      
        if(IPs_Liste[i]==IP_Courante[3]){
          etat_course[i][NbCP-1]= temps_lecture_tag ;
        }
      }
      classement2();

    }
    }
  }
  
  ws.cleanupClients();
   
}
