/*
    Description: Use GoPLUS Module for four-channel servo test and three-channel ADC test.
*/
#include <M5Stack.h>
#include <Wire.h>
#include "GoPlus2.h"
#include <math.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>


const char* ssid = "MONSUPERARDUINO";
const char* password = "PASSWORD1234";


GoPlus2 goPlus;

#define X_LOCAL 40
#define Y_LOCAL 30

#define X_OFFSET 160
#define Y_OFFSET 23
#define MAXSPEED 127
#define LIMITTURN 0.01


int _hub1, hub1 = 0;

size_t received = 0;

int speed1 = 0;
int speed2 = 0;

int flag = 0;
int num = 0;


AsyncWebServer server(80);
AsyncWebSocket ws("/ws");


const char index_html[] PROGMEM = R"rawliteral(
<html lang="en">
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
      margin: 0px;
    }

    canvas {
      background-color: #111133;
      display: block;
      position: absolute;
    }

    .container {
      width: auto;
      text-align: center;
      background-color: #ff0000;
    }
  </style>
</head>
<body>
    <div class="container">
        <canvas id="canvasSurfaceGame"></canvas>
        <p class="state">state: <span id="speed">%SPEED%</span></p>
    </div>
</body>

<script>
var gateway = `ws://${window.location.hostname}/ws`;
var websocket;
var isstarted=false;
var x=0;
var y=0;
var speed1=0.0;
var speed2=0.0;
var limiter=0;
var speedmodified=false;
var date = Date.now();

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
const packetlimit=100;

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
    requestAnimFrame(draw);
}

function resetCanvas(e) {
    // resize the canvas - but remember - this clears the canvas too. 
    canvas.width = window.innerWidth;
    canvas.height = window.innerHeight;

  thirdWidth= canvas.width/3;
  thirdHeight= canvas.height/3;
  twothirdWidth= (2*canvas.width)/3;
  twothirdHeight= (2*canvas.width)/3;

    //make sure we scroll to the top left. 
    window.scrollTo(0, 0);
}


function draw() {
    c.clearRect(0, 0, canvas.width, canvas.height);

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

    
  if (isstarted && (Date.now()-date)>packetlimit){
    //console.log(x+"-"+y);
    
    websocket.send("SPEED:"+speed1+":"+speed2);
    date = Date.now();
  }
  
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
    //fonction d'appel websocket objet
    }
    touches.add(e.pointerId, newPointer);
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
        x=0;
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
  function onMessage(event) {
    document.getElementById('speed').innerHTML = event.data;
  }
  function onLoad(event) {
    initWebSocket();
    initButton();
  }
</script>
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
  ws.textAll(String(speed1)+"-"+String(speed2));
}

void handleWebSocketMessage(void *arg, uint8_t *data, size_t len) {
  AwsFrameInfo *info = (AwsFrameInfo*)arg;
  if (info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT) {
    data[len] = 0;
    char **donnee = str_split((char *)data,':');
    if (strcmp(donnee[0],"SPEED") == 0) {
      speed1=MAXSPEED*(strtof(donnee[1],NULL));
      speed2=MAXSPEED*(strtof(donnee[2],NULL));
      //Serial.printf("vitesse 1 : %d\n",speed1);
      //Serial.printf("vitesse 2 : %d\n",speed2);
      notifyClients();
    }
  }
}

void onEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type,
             void *arg, uint8_t *data, size_t len) {
  switch (type) {
    case WS_EVT_CONNECT:
      //Serial.printf("WebSocket client #%u connected from %s\n", client->id(), client->remoteIP().toString().c_str());
      break;
    case WS_EVT_DISCONNECT:
      //Serial.printf("WebSocket client #%u disconnected\n", client->id());
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

String processor(const String& var){
  Serial.println(var);
  if(var == "SPEED"){
    return (String(speed1)+"-"+String(speed2));
  }
}





void doTask()
{
  if(num == 4){
    num = 0;
  }else{
    num++;
  }
  M5.Lcd.println(num);
}

void forward(int speedOne,int speedTwo){
  goPlus.Motor_write_speed(MOTOR_NUM0, speedOne); // valeur inversé moteur num0 : moteur droit
  goPlus.Motor_write_speed(MOTOR_NUM1, -speedTwo*0.95); // moteur gauche
}

void printWifiStatus() {

  // print the SSID of the network you're attached to:

  Serial.print("SSID: ");

  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address:

  IPAddress ip = WiFi.localIP();

  Serial.print("IP Address: ");

  Serial.println(ip);

  // print the received signal strength:

  long rssi = WiFi.RSSI();

  Serial.print("signal strength (RSSI):");

  Serial.print(rssi);

  Serial.println(" dBm");
}

void setup() {

  M5.begin();
  Serial.begin(115200);
  goPlus.begin();
  delay(100);

  
   // Connect to Wi-Fi
  WiFi.mode(WIFI_AP);
  WiFi.softAP(ssid, password);
  WiFi.setSleep( false );
  
  IPAddress IP = WiFi.softAPIP();
  WiFi.begin();
  // Print ESP Local IP Address
  printWifiStatus();
  M5.Lcd.println(IP);
  M5.Lcd.qrcode("WIFI:S:MONSUPERARDUINO;T:WPA;P:PASSWORD1234;;",0,0,200,6);

  /*

    WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }

  // Print ESP Local IP Address
  Serial.println(WiFi.localIP());
  M5.Lcd.println(WiFi.localIP());
  */
  initWebSocket();
  // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html, processor);
  });

  //print du QRcode de wifi
  //M5.Lcd.qrcode("WIFI:S:MONSUPERARDUINO;T:WPA;P:PASSWORD1234;;",0,0,200,6);
  //M5.Lcd.println(IP);

  // Start server
  server.begin();


//   goPlus.hub1_set_io(HUB1_R_O_ADDR, 1); //set digital_output to digital_input
//   goPlus.hub2_set_io(HUB2_R_O_ADDR, 1); //set digital_output to digital_input
//   goPlus.hub3_set_io(HUB3_R_O_ADDR, 1); //set digital_output to digital_input
}

void loop() {
   ws.cleanupClients();
   forward(speed1,speed2);

  
//    int val1 = goPlus.hub1_d_o_read_value(HUB1_R_O_ADDR);  //read digtial_input
//    int val2 = goPlus.hub2_d_o_read_value(HUB2_R_O_ADDR);  //read digtial_input
//    int val3 = goPlus.hub3_d_o_read_value(HUB3_R_O_ADDR);  //read digtial_input
//    Serial.println(val1);
//    Serial.println(val2);
//    Serial.println(val3);

}
