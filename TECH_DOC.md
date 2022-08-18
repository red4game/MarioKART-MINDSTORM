# **Documentation technique**

## **Langage Javascript**
### **librairies utilisées**
>(function(){if(!Array.prototype.indexOf){Array.prototype.indexOf=...

librairie handjs minifiée à la ligne 210 nous permettant de gérer les différentes interractions entre l'écran et soit une souris ou bien un écran tactile. Cette librairie nous permet d'avoir des interactions avec nos joysticks ne manière fluide. Vous pouvez regarder la version non minifiée sur ce github https://github.com/CognitiveScale/handjs

```
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
```
librairie situé entre les lignes 213 et 238 permettant de simuler une collection pour manipuler une liste d'objet plus facilement avec des fonctions comme add, remove, item ou forEach 4 fonctions de base pour pouvoir gérer la liste d'objet avec des clés données

add : on lui donne une clé et un item et l'ajoute dans la collection
remove : on lui donne une clé et il retire l'objet associé à la clé
item : on lui donne une clé et il renvoie l'item associé à la clé
forEach : on lui donne une fonction et il appliquer la fonction donnée en paramètre à tout les items 


```
var Vector2 = function (x,y) {
  this.x= x || 0; 
  this.y = y || 0; 
};
```

librairie permettant de créer un objet vecteur qui possède simplement 2 attributs : x et y 
cette librairie possède de nombreuse fonctions utilitaire pour faire du calcul sur un objet vecteur avec un prototype définit entre les lignes 251 et 45. Voici les fonctions présente dans le prototype de l'objet Vector2 :

>toString : function (decPlaces)

renvoie le vecteur sous la forme d'un string affichable avec en paramètre le nombre de chiffre après la virgule pour les coordonées 

>clone : function ()

renvoie simplement un nouveau vecteur identique à celui traité en objet, littérallement un clone comme en java

>copyTo : function (v)

va venir prendre un vecteur v en paramètre et venir copier les coordonées de l'objet pourlequel la fonction est utilisé et copier ses coordonées dans le vecteur v

>copyFrom : function (v)

processus inverse de copyTo on prend notre vecteur d'origine et on lui met les coordonées de notre vecteur v mis en paramètre

>magnitude : function ()

renvoie la distance du vecteur, c'est a dire sqrt(x²+y²)

>magnitudeSquared : function ()

renvoie la distance au carré du vecteur, c'est à dire x²+y²

>normalise : function ()

renvoie un vecteur de même direction que le vecteur d'origine mais avec une distance de 1: un vecteur unitaire

>reverse : function ()

inverse les coordonées du vecteur, x devient -x et pareil pour y

>plusEq : function (v)

addition de coordonées de vecteur sur le vecteur auqel on a appelé la fonction

>plusNew : function (v)

addition de coordonées de vecteur et produit un nouveau vecteur

>minusEq : function (v)
>minusNew : function (v)
>multiplyEq : function (scalar)
>multiplyNew : function (scalar)
>divideEq : function (scalar)
>divideNew : function (scalar)

mêmes fonctions que plus mais cette fois ci pour la soustraction de vecteurs, multiplications et divisions scalaires avec eq qui effectue sur le vecteuyr en cours et new dans un nouvel objet vecteur

>dot : function (v)

produit scalaire de vecteurs

>angle : function (useRadians)

renvoie l'angle trigonométrique du vecteur, si useRadians est à true alors renvoie en radians sinon renvoie en degrés.

>rotate : function (angle, useRadians)

fait une rotation du vecteur donnée et vient changer directerment ses coordonées tout en gardant la même distance, et si useRadians est à true alors on prend la variable angle en radians et sinon en degrés

>equals : function (v)

vérifie l'égalité entre deux vecteurs et renvoie un booléen

>isCloseTo : function (v, tolerance)

on vient faire la différence des deux vecteurs et on vérifie la distance du vecteur résultant avec une tolérance donné, si on est en dessous niveau longueur de la tolérance alors on renvoie true et sinon on renvoie false 

>rotateAroundPoint : function (point, angle, useRadians)

fait une rotation du vecteur autour d'un point avec point étant un objet vecteur possédant donc des coordonées précises et userRadians permettant toujours de signifier avec un booléen l'utilisation ou non de radians pour l'angle de rotation.

>isMagLessThan : function (distance)
>isMagGreaterThan : function (distance)

fonctions de comparaisons avec la distance d'un vecteur


```
Vector2Const = {
  TO_DEGREES : 180 / Math.PI,   
  TO_RADIANS : Math.PI / 180,
  temp : new Vector2()
  };
```
objet contenant certaines constantes utiles pour les fonctions au dessus nottament pour les conversions degrés/radians 

### **Partie stylistique CSS**

```
    * {
    -webkit-touch-callout: none; /* prevent callout to copy image, etc when tap to hold */
    -webkit-text-size-adjust: none; /* prevent webkit from resizing text to fit */
    /* make transparent link selection, adjust last value opacity 0 to 1.0 */
    -webkit-tap-highlight-color: rgba(0,0,0,0);
    -webkit-user-select: none; /* prevent copy paste, to allow, change 'none' to 'text' */
    -webkit-tap-highlight-color: rgba(0,0,0,0);
    touch-action: none;
    }
```

bloc utile pour la définition de divers paramètres utiles pour handJS pour avoir la meilleur expérience possible au niveau du tactile sur téléphone afin d'enlever toute les précédentes interactions qui ferraient conflit avec nos interactions

```
   .leftFooter, .rightFooter, .middleFooter {
    height:100%%;
    flex: 1 1 30%%; /*grow | shrink | basis */
    border: 1px solid;
   }
```

Balise de style util;e pour la séparation en  3 colonnes distinctes dans la page HTML


```
  .footer{
    background:orange;
    position: fixed;
    left: 0;
      bottom: 0;
    width:100%%;
    height:20%%;
    text-align: center;
  }
```

Balise de style pour la barre d'information du bas de l'écran.


Concrétement je vous conseille pour cette partie de CSS de modifier à votre guise et de regarder les différents comportement, en effet le CSS est un petit peu brouillon pour l'instant et il faudrait clarifier l'utilité de certains blocs.


### **Partie Affichage HTML**
Ensuite il nous faut parler de l'utilité du body HTML : il se compose de deux éléments principaux: 
```
    <div class="container">
        <canvas id="canvasSurfaceGame"></canvas>
    </div>
```

C'est un container qui contient un simple canvas d'id "canvasSurfaceGame",  c'est cet identifiant qui va nous servir à faire la liaison avec notre partie logique et graphique en JavaScript. Rien de plus à dire, on a fait en sorte que celui ci prenne tout le containeur et d'adapter ainsi le containeur à l'endroit où on en avait besoin.

```
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
```

on a un objet footer qui nous permet de définir une zone d'information en bas, avec un DIV qui contient lui même 3 DIV avec des classes différentes pour pouvoir y gérer indépendemment le style graphique. De plus on remarque la présence de balises P, avec à l'intérieur des SPAN, ces SPAN ont des ID, c'est là où on va faire remontter nos informations directement du WEBSOCKET avec la fonction processor : par exemple %MAVARIABLE% indiquera que l'on doit y placer une variable qui se nomme MAVARIABLE dans la fonction processor qui va venir initialiser les valeurs dans ces champs là. les ID nous servent à modifier dynamiquement ensuite les valeurs tout au long de la course une fois que l'utilisateur a déjà charger la page avec le code suivant javascript : 
>document.getElementById('mavriable') = texte informatif


### **Partie traitement JavaScript**
Maintenant nous allons parler de toute la partie logique aussi bien du canvas que de la reception des messages au nvieau du WEBSOCKET entre les lignes 550 et 897.

```
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
```

voici une fonction permettant d'uniformiser les animations en fonction des navigateurs afin d'avoir une animation au mieux pour chaque navigateur. je ne vous conseille pas de la changer mis appart peut être moduler la dernière partie window.setTimeout en changeant le frameramte au niveau du deuxième argument si vous en avez envie ou besoin

```
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


document.addEventListener("DOMContentLoaded", init);

window.onorientationchange = resetCanvas;
window.onresize = resetCanvas;
```

voici la définition des variables globales utiles pour la gestion de notre controller en javascript je vous y décrire toute les variables leur utilité

>var gateway = \`ws://${window.location.hostname}/ws\`;

variable permettant de spéciifer le chemin à utiliser pour la communication websocket, ici vous avez un websocket qui est sur la même machine qui hébèrge le web donc vous êtes en local d'où le ${window.location.hostname}

> var websocket;

variable qui va contenir l'objet websocket qui va être initialiser au moment de l'appel de la fonction initWebSocket()

```
  function initWebSocket() {
    console.log('Trying to open a WebSocket connection...');
    websocket = new WebSocket(gateway);
    websocket.onopen    = onOpen;
    websocket.onclose   = onClose;
    websocket.onmessage = onMessage; // <-- add this line
  }
```

cette fonction va venir indiquer qu'on ouvre le web socket avec pour path la gateway définiti juste au dessus, on instancie notre objet et on vient redéfinir les fonctions onopen, onclose  et onmessage qui seront des fonctions de callback au moment d'un événement respectivemnet à l'ouverture, la fermeture et l'envoi de message à travers le websocket. voici en bas nos fonctions définis dans le javascript 

```
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
```

> var isstarted=false;

c'est une variable booléenne qui est vérifiée à chaque fois que 'lon envoie un message au niveau du websocket. En effet lorsque le websocket est démarré on place la variable à true qui est à false de base et signifie qu'on peut envoyer des messages à travers le socket. Ainsi quand on envoie un message on vérifie de le websocket est ouvert 

```
var x=0;
var y=0;
var speed1=0.0;
var speed2=0.0;
```

les variables x et y sont des variables déduites des coordonées de nos joysticks, ainsi x est définit par notre joystick de gauche pour définir la direction du kart et y pour définir l'acceleration du kart. Ensuite à partir des coordonées de déplacement du joystick on peut en déduire les vitesses grâce à la partie de code suivante
 
```
  if(x<0){
    speed1=parseFloat(((1.0)+x)*y);
    speed2=parseFloat(y);
  } else if (x>=0){
    speed1=parseFloat(y);
    speed2=parseFloat(((1.0)-x)*y);
  }
```

>var speedmodified=false;

cette variable nous sert à signifier ou non si l'on a eu un changement de vitesse des moteurs pour qu'on ne renvoie pas 2 fois la même vitesse au websocket afin de limiter l'envoi des données et donc la surcharge du websocket.

>var packetcounter=0;

> const packetlimit=8;

ces 2 variables servent à limiter l'envoi de packet au websocket, ici notre packet counter est incrémenter à chaque frame et remis à 0 à chaque fois qu'on envoie un paquet, ainsi notre packet limiter nous indique tout les combien de frame on peut envoyer un paquet à travers le websocket.


```
var myleftnewx=0.0;
var myleftnewy=0.0;
var mynewx=0.0;
var mynewy=0.0;
```

ce sont les coordonées de nos vecteurs associés à noitre joystick gauche si il y a left dans la variable et sinon au joystick droit.

```
var canvas,
c, // c is the canvas' context 2D
```

ici on a des variables qui sont utile pour définir notre canvas. En effet le canvas est récuperer dans la variables canvas puis le contexte y ai mit dans la variable C. Nous feront tout nos traitements sur la variable c.

```
function setupCanvas() {
    canvas = document.getElementById('canvasSurfaceGame');
    c = canvas.getContext('2d');
    resetCanvas();
    c.strokeStyle = "#ffffff";
    c.lineWidth = 2;
}
```

```
thirdWidth,
thirdHeight,
twothirdWidth,
twothirdHeight,
```

Ces variables servent à définir la tailles de nos zones d'actions et sont redéfinis en fonction de la taille de l'écran et ce à chaque modification de l'écran comme montré dans la fonction ci dessous 

``` 
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
```

```
leftPointerID = -1,
leftPointerPos = new Vector2(0, 0),
leftPointerStartPos = new Vector2(0, 0),
leftVector = new Vector2(0, 0),
rightPointerID = -1,
rightPointerPos = new Vector2(0, 0),
rightPointerStartPos = new Vector2(0, 0),
rightVector = new Vector2(0, 0),
middlePointerID=-1;

var touches; // collections of pointers
var newPointer = { identifier: e.pointerId, x: e.clientX, y: e.clientY, type: givePointerType(e) }
```

ces variables sont des variables utiles pour la gestions de nos joystick et du bouton du milieur en effet on a pour chaque joystick et bouton un id associés qui sert de clé dans notre collection touches qui contient des variables de type newPointer avec les valeurs de la librairie handJS.

ensuite pour les joysticksw on a 3  vecteurs associés par joystick à savoir un vecteur de position initial, un vecteur de position actuel et un vecteur résultant de la différence des deux autres avec position actuelle moins la position de départ.

```
var objet;
var aObjet=false;
var myImage=new Image();
```

ce sont les variables utiles pour stocker un objet, vérifier qu'il y a un objet présent et enfin l'image associées à l'objet.

>const rayon=50;

efnin on a une constante de rayon qui définit le rayon maximal auquel on peut aller avec notre joystick, cett evaleur est modifiable mais à été mise à 50 pour pouvoir jouer convenablement avec un téléphone en mode paysage.

Maintenant que toutes les variables ont été définit il ne nous reste plus qu'à voir comment est ce qu'on effectue notre traitement.

pour commencer au départ on a une fonction d'initialisation du canvas :

```
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
```

dans cette fonction d'init on va venir définir nos événements à savoir pointerDown quand on appuie sur l'écran, pointmove quand on bouge notre doigt sur l'écran et pointerup et pointerout quand on retire notre doigt de l'écran. De plus on vient initialiser notre image d'objet avec un cube objet de mariokart et enfin on vient appeler requestAnimFrame(draw) avec en fonction de callback notre fonction draw qui va venir afficher nos élément du canvas avec un framerate définit par la fonction requestAnimFrame


```
function draw() {
    c.clearRect(0, 0, canvas.width, canvas.height);
    c.drawImage(myImage,thirdWidth,0,thirdWidth,thirdWidth);
    touches.forEach(function (touch) {
        if (touch.identifier == leftPointerID) {
            ...
        }         
    if (touch.identifier == rightPointerID) {
           ...
        }
    else {
      //si on a envie
        }
    });

    
  if (isstarted && packetcounter>=packetlimit && speedmodified){
    //console.log(x+"-"+y);
    
    websocket.send("SPEED:"+speed1+":"+speed2);
    date = Date.now();
    packetcounter=0
  }
  packetcounter++;
  
  requestAnimFrame(draw);
}
```

Ici notre fonction draw va être appelée à chaque frame et on envoie à travers le websocket les variables speed1 et speed2 qui ont été calculés à chaque fois que l'on bouge un joystick. Ensuite on va venir itérer sur notre Collection touches avvec un forEach et pour chaque élément on va venir vérifier l'identifiant, si jamais l'identifiant correspond à celui du joystick droit alors on va venir désseiner nos cercles avec les variables associés à notre joystick droit pour pouvoir littérement dessiner notre joystick droit sur notre canvas à la bonne position, un exemple ci dessous avec le joystick droit.

```
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
```

On va venir dessiner des cercles de couleur bleu, 2 cercles avec un fixe au niveau de la position de départ et un deuxième plus petit qui va vnir bouger en fonction du déplacement de notre doigt.


> function onPointerDown(e)

c'est la fonction qui va nous servir à ajouter nos interractions à la collection Touches lorsque l'on va toucher l'écran. Si c'est c'est un joystick alors on va venir initialiser les bonnes variables avec pour position du joystick l'endroit où l'on a appuyer sur l'écran et si c'est le bouton du milieu alors on vérifie si l'on a un objet et on l'envoie à travers le websocket comme ci dessous
```
if(aObjet ==true){
  myImage.src="https://www.nicepng.com/png/full/154-1548752_image-result-for-item-box-mario-kart-wii.png";
  websocket.send(objet);
  aObjet =false;
}
```

>function onPointerMove(e)

Ici c'est une fonction qui est appellée à chaque fois que l'on bouge notre doigt sur l'écran et donc va nous servir à bouger en fonction de notre id et de la position de notre doigt les joysticks sur l'écran tout en respectant une limite de rayon du joystick mis en constante. C'est aussi cette fonction qui va nous permettre d'actualiser nos variables x et y et donc de calculer nos vitesses speed1 et speed2

>function onPointerUp(e)

lorsqu'on sort de l'écran ou qu'on lève notre doigt cette fonction est appellé, on enlève nos joysticks de la collection Touches et on reinitialise la position des vecteurs associés et en remettant ainsi nos vitesses à 0.




## **Langage C**
### **Les variables**

>\#define N

Nombre max de karts dans le jeu

> const char* ssid = "LUIGI_WIFI";

Ssid héberger par le M5Stack
> const char* password = "mariokartmindstorm";

Mot de passe du WiFi
> unsigned long temps_lecture_tag; 

Sert a stocker le temps de lecture d'une tag NFC
> char* sender;

Déclaration d'un message normé pour l'établissement de la connexion définit ligne 1445
>char* receiver;

Déclaration d'un message normé pour l'établissement de la connexion définit ligne 1446
>int nbJoueurs =1;

Initialisation du nombre de joueur à 1 car on joue
>int nbCheckpoints = 9;

Nombre de checkpoint dans le jeu (3 tous de 3 checkpoint)

>char* ids_CP1[7]={"b3d4fe2a","6c4c312","36e6e3ad","16bd5b13","70b670a5","56f6ffad","563f6dc"}; 
>char* ids_CP2[7]={"562a36c6","3372522b","96434b73","c342792b","863a48af","13d76f2b","43b14a2b"};
>char* ids_CP3[7]={"96fb7513","63b48f24","f38a562b","6c7efad","e6c452c","86b0ed12","f68bd0c"};
>char* objets_ids[4]={"a6115f13","86c92b65","e72e7775","90fd81a5"};

La liste des ids des tag NFC dans les checkpoints et les objets

String chaine_a_envoyer = "";
>int IPs_Liste[N]; 

Initialisation de la liste des IPs des participants du jeu, on ne sauvegarde que les 3 derniers chiffres de l'IP car on est sur le meme réseau
>int rank[N];    

Initialisation du classement, meme systeme de stockage de l'IP que pour IPs_Liste
>unsigned long  etat_course[N][100]; 

Un tableau qui donne pour chaque adresse IP et chaque checkpoint le temps auquel le checkpoint à été passé par le joueur correspondant à l'IP.
>int NbCP;

Nombre de checkpoints passés par le kart courrant.

>WiFiUDP ntpUDP;
>NTPClient timeClient(ntpUDP,"fr.pool.ntp.org", 3600, 1000);

Déclaration permettant de récupérer le temps dans le programme
>IPAddress IP_Courante;

Déclaration de la variable contenant l'IP courrante initialisée ligne 1438


>typedef struct{ 
>    int ack;
>    String donnee;
>    int nbEnvoi;
>    IPAddress ip;
>    unsigned long myTime;
>  } Message;

Strucutre d'un message envoyé en utilisant le protocole UDPPlus qu'on à réimplémenté.

>int maxRepetition=5;                

Nombre de réenvoi de message avant l'abandon des réenvois
>int sequence =1;

Initialisation du numéro de séquence -> voir fonctionnement du protocole TCP
>int maxqueue =100;

Taille maximum de message en transit
>int messagesSize =0;

Nombre de message en transit
>Message messages[100];

Initialisation du tableau contenant les messages envoyés


>unsigned long rtt = 2000;   

Definition du RTT, temps avant réenvoi de la requete
>unsigned long paralizedAt=millis();

Temps auquel on a reçu une carapace -> sert pour calculer depuis combine de temps on est pénalisé
>unsigned long tempsParalized = 3000;

Durée de l'imobilisation
>boolean paralized =false;

Boolean pour savoir si on est actuellement paralisé

>char* objets[2] = {"carapaceRouge","carapaceVerte"};
>int nbObjets =2;

Liste des objets et sa taille.

>char * messCarapaceRouge="cr";
>char * messCarapaceVerte ="cv";
>char * messCheckPoint="cp";

Messages correspondant à la reception d'une carapace rouge ou verte ou d'un checkpoint

>int probaCarapaceVerte = 50;

Probabilité en pourcentage que la carapace verte touche l'adversaire.

>int speed1 = 0;
>int speed2 = 0;

Valeur de vitesse qur chacun des moteurs

>String RFIDvalue;

Valeur lu par le capteur RFID
>String lastRFID;

Valeur précédemment lu par le capteur RFID pour éviter de traiter le meme tag lu plusieurs fois de suite.

>AsyncUDP udp;

Initialisation de l'objet permettant la communication udp

>AsyncWebServer server(80);
>AsyncWebSocket ws("/ws");

Initialisation du WebSocket TCP sur le port 80

### **Les fonctions**
#### str_split
> char** str_split(char* a_str, const char a_delim)

Permet de rendre tableau de chaine de caractère, grace au délimiteur
Exemple : str_split("1/2/3/4", "/") -> [1,2,3,4]
#### updateClassement
> void updateClassement()

Permet d'envoyer un message à l'interface utilisateur avec le webSocket contenant le nouveau classement. 

#### updateCheckpoint
> void updateCheckpoint()

Permet d'envoyer un message à l'interface utilisateur avec le webSocket contenant le nouveau checkpoint.

#### maxofarray
> int maxofarray(int a[],int n)

Fonction qui retourne l'indice du max d'un tableau

#### minofarray2
> int minofarray2(long a[][2],int n)

Fonction qui retourne l'indice du min d'un tableau

#### classement2
> void classement2()

Fonction qui a partir du tableau etat_course construit le classement dans le tableau rank

Explication de l'algorithme :
TODO

#### resetWifi
> void resetWifi()

Permet de réinitialiser la configuration WiFi enregistrée

#### forward
> void forward(int speedOne,int speedTwo)

Permet de controller les moteurs du kart, s'occupe aussi de fare tourner le kart en rond si jamais il est paralisé (touché par unee carapace)

#### carapaceVerte
> void carapaceVerte()

Reactio à la reception d'une carapace verte

#### carapaceRouge
> void carapaceRouge()

Reactio à la reception d'une carapace rouge

#### sendUdpPlus
> void sendUdpPlus(String donnee,IPAddress ipAdd)

Permet d'envoyer un message en utilisant notre protocole udp modifié, avec en paramètre la donnée et l'IP de la cible

#### checkUdpRtt
> void checkUdpRtt()

Vérifie pour les messages envoyés si le RTT n'a pas été dépassé et si il est dépassé réenvoie ce message

#### checkParalized
> void checkParalized()

Vérifie que le temps de paralisation n'est pas dépassé

#### initWebSocket
> void initWebSocket()

Initialise le webSocket

#### onEvent
> void onEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type,
             void *arg, uint8_t *data, size_t len)

Permet de réagir aux évènements du webSocket

#### handleWebSocketMessage
> void handleWebSocketMessage(void *arg, uint8_t *data, size_t len) 

Traitement des messages recus par le webSocket

#### setup
>void setup()

Fonction obligatoire de Arduino qui est lancée à chaque démarrage en première. Elle sert globalement à initialiser tout correctement avant le "vrai" démarrage du programme.

#### loop
> void loop()

Fonction obligatoire de Arduino qui est lancée après le setup. C'est ici qu'est écrit le progamme qui permet de faire tourner le jeu coté M5Stack. Comme son nom l'indique, cette fonction tourne en boucle.

### **Explication de la fonction Setup**


```M5.begin(); 
Serial.begin(115200); 
goPlus.begin(); 
mfrc522.PCD_Init();
M5.Lcd.begin();
```

Initialisation des différents objets gérant le M5Stack, la communication serie entre la M5Stack et l'ordinateur si ils sont branchés, le goPlus2, le capteur RFID et l'écran du M5Stack.
```
 for(int i=0;i<N;i++){
    IPs_Liste[i] = -1;
 }
```
Initialisation de la liste d'IP.

>boolean bistable=false;

Sert à déterminer si la connexion au WiFi c'est bien passée.

```M5.Lcd.qrcode("WIFI:S:LUIGI_WIFI;T:WPA;P:mariokartmindstorm;;",0,0,220,6);
  wm.setWiFiAPChannel(8);
  wm.setConnectRetries(5);
  wm.setConnectTimeout(3);
  WiFi.mode(WIFI_STA)
  ``` 

Parametrage du WiFi, channel 8 pour éviter les interférences, etc.

  ``` 
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
``` 

On héberge un point d'accès WiFi avec le M5Stack, l'utilisateur peut se connecter dessus et rentrer les credentials de son WiFI pour que le M5Stack se connecte dessus. Après le M5Stack redemarre et essaie de se connecter, si il y arrive pas ça recommence et sinon il garde en mémoire les credentials du WiFi (appuie sur la touche du milieu sous l'écran pour ecraser la mémoire).
``` 
  IP_Courante = WiFi.localIP();
  IPs_Liste[0] = IP_Courante[3];
  rank[0] = IP_Courante[3];
``` 

On récupère notre Ip sur le réseau et on se place dans les liste de rank et d'IP.

> timeClient.begin();

Démarrage du client qui sert à récupérer l'heure grace au serveur NTP.

>sender = "Are you here Mario?";  
>receiver = "Its Me Mario!";

Deux messages normalisés dans notre protocole de connexion qui permettent aux karts de se reconnaitre sur le réseau.

>if(udp.listen(42424)) {
>    udp.onPacket([](AsyncUDPPacket packet) {

Permet d'écouter sur le port 42424 en udp et si on reçoit quelque chose on exécute la fonction en paramètre.

```
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
```

Si on reçoit un message "Its Me Mario!", on ajoute l'ip (uniquement les 3 derniers chiffres) dans notre liste d'ip et on la trie par nombre croissant. On copie le rank sur la liste parce qu'il faut bien un classement initial.

```
Serial.println(" Ma liste d'IP");
            for(int i=0;i<nbJoueurs;i++){
              Serial.print(" |");
              Serial.print(IPs_Liste[i]);
              Serial.print("| ");
            }
            Serial.println("");

```
Affichage pour que vous puissiez voir si tout est ok.

```
initWebSocket();
  // Route for root / web page
  server.on("/game/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html, processor);
    updateCheckpoint();
  });
```
On initialise le webSocket et on configure la route pour acceder à l'interface web.

>server.begin();

On démarre le server web.

>lastRFID=String("FFFFFFFFFFFFFFFFFFFF");
  RFIDvalue=String("FFFFFFFFFFFFFFFFFFFF");

On doit les initialiser en dur au cas ou on devrait les comparer avant d'avoir lu un tag.

### **Explication de la fonction Loop**

>if (M5.BtnB.isPressed()){
    resetWifi();
  }

On peut reset le WiFi en appuyant sur le bouton du milieu.

>forward(speed1,speed2);

On met à jour les moteurs avec la vitesse, 0 si on a rien reçu de l'interface utilisateur et sinon la valeur retournée par l'interface utilisateur.

>  checkUdpRtt();

On vérifie que aucun de nos RTT ne soit expiré
>  checkParalized();

On vérifie qu'on si on est toujours paralisé (si c'est le cas)

> if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial() )

Si le capteur est passé sur un nouveau tag NFC et qu'on l'a bien lu alors ....

```
    timeClient.update();
    unsigned long temps_lecture_tag=timeClient.getEpochTime();
    lastRFID=String("");
    
    for (int l=0;l<mfrc522.uid.size;l++){
      lastRFID += String(mfrc522.uid.uidByte[l], HEX);
    }
```
On met à jour le temps auquel on a lu le dernier tag et on transforme ce qu'on lit du tag en un String (plus simple pour les comparaisons)

> if (!RFIDvalue.equals(lastRFID))

Si c'est un nouveau tag (normalement on passe toujours dans cette condition car "mfrc522.PICC_IsNewCardPresent()" avant).

>if(Contains_Tab(objets_ids,(char*)RFIDvalue.c_str(),4)){
        ws.textAll(objets[random(0,nbObjets)]);
      }

On vérifie si le tag est dans la liste de ceux correspondant aux objets.

> if(Contains_Tab(ids_CP1,(char*)RFIDvalue.c_str(),7) && NbCP%3==0)

On cérifie si le tag est dans la liste de ceux correspondant au checkpoint 1 et que l'on est au checkpoint 0. Il y a les meme conditions pour les checkpoints suivant.

```
NbCP++;
updateCheckpoint();
chaine_a_envoyer=String("cp:")+String(NbCP)+String(":")+String(temps_lecture_tag);for(int k=0;k<nbJoueurs;k++){ 
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
```
On incremente le nombre de checkpoint passés, puis on en informe l'interface web.
Ensuite on construit le message à envoyer aux autres karts, puis on envoie avec notre protocole. On ajoute l'information avec le temps dans le tableau traduisant l'état de la course et enfin on met à jour le classement.

## **Nous contacter**

Si jamais vous avez besoin de nous contacter pour plus d'informations ou quoi que ce soit :
```
Augereau Jeremy       Jeremy.Augereau@insa-rennes.fr
Bendali Youssef       Youssef.Bendali-Yahya@insa-rennes.fr
Caisson Mathis        Mathis.Caisson@insa-rennes.fr
Cheddad Fatine        Fatine.Cheddad@insa-rennes.fr
```
