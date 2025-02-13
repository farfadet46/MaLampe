/*         
 * ---------------------------------------------------------------------------------------------------------
 * Les petits Débrouillards - décembre 2022 - CC-By-Sa http://creativecommons.org/licenses/by-nc-sa/3.0/
 * =========================================================================================================

 // Bibliothèques WiFi et WebServer: ATTENTION, choisir celles correspondant à votre matériel. 
 // ATTENTION AUX MAJUSCULES & MINUSCULES ! Sinon d'autres bibliothèques, plus ou moins valides, seraient utilisées.
*/

#include <ESP8266WiFi.h>                // A utiliser pour le D1 Mini 
#include <ESP8266WebServer.h>           // A utiliser pour le D1 Mini

const char* mySSID = "Ma_Lampe" ;    // On va utiliser le mode "Access Point" pour cet exemple
//const char* mySecKey = "alien" ;

// Déclaration de notre serveur web interne, qui écoutera sur le port 80.

ESP8266WebServer myWeb(80);           // A utiliser pour le D1 Mini

// Variable qui sera affichée et modifiée depuis notre interface web.
int brightness = 255 ;
const int LedPin = 4; //LED sur GPIO4

/* --------------------------------------------------------------------------------------------------------
 *  webPage01 : formattage HTML de la page web. 
 *  - En fait cette fonction doit rendre une chaîne (String) contenant l'intégralité du code HTML qui sera
 *    envoyé au navigateur de l'utilisateur.
 *  - Comme dans l'exemple précédent (Exemple_4A), on insère dans cette chaîne une information de la carte,
 *    ici la valeur de notre variable 'my value'. Mais on va aussi ajouter des boutons permettant de modifier
 *    cette valeur sur la carte. 
 * - Idem Exemple_4A : pour pouvoir débugger facilement le code HTML/Javascript sur un browser (par exemple 
 *   Firefox / Outils supplémentaires / Outils de développement Web), il est préférable d'indenter le code à
 *   l'intérieur de la chaîne de caractère, et de mettre des sauts de ligne ("\n") à la fin de chaque ligne 
 *   de code HTML. 
 *  -------------------------------------------------------------------------------------------------------- */

String webPage01() {
    String p;  
    // Début de construction de la page web (entête, titre, paramètres)
  
    p =  "<html lang=fr-FR><head>\n" ; 
    p += "<title>ma lampe</title>\n" ;              // Titre de la page
    p += "    <meta charset='UTF-8'>\n" ;           // Codage des caractères, UTF-8 est fortement recommandé
    p += "</head>\n" ;

    // Définitions CSS (), qui permettent de décrire le format des objets sur la page web. 
    // Si vous voulez tout savoir sur CSS, on peut trouver une bonne introduction ici : https://developer.mozilla.org/fr/docs/Learn/CSS
    // et une référence complète ici : https://developer.mozilla.org/fr/docs/Web/CSS/Reference
   

    p  += "<style>\n" ;
    p += "    body { background-color: #1a2025; color: white; font-size: 25px; }\n";           // couleur fond écran (bleu foncé) et textes (blanc).
    p += "    input { width:200; margin:10px; font-size:20px; border-radius: 25px; }\n";        // format des boutons (taille, coins arrondis, ...).
    p += "</style>\n" ;


    // Début du code javascript. Javascript est le langage utilisé au niveau des navigateurs web (Firefox, Microsoft Edge, Google Chrome, ...)
    // pour introduire un peu de dynamisme et d'intelligence dans les pages web. Cela peut permettre, par exemple, de réaliser une action 
    // locale et immediate, telle que l'agrandissement d'une image, le changement d'un texte, etc ... sans avoir à réinterroger le serveur web.
    // 
    // Dans notre cas, la fonction 'addition(val)' ci-dessous va ajouter le paramètres 'val' à l'adresse du serveur web, et va ensuite appeler
    // la page web de notre carte, avec ce paramètre. Par exemple, si l'adresse du site web de notre carte est 192.168.4.1, l'appel à la fonction
    // addition(-1) va demander la page '192.168.4.1?add=-1'. Le paramètre 'add' de valeur '-1' sera alors exploité par la carte dans la 
    // fonction runPage01() définie plus bas. 
    //
    // Dans un exemple réel on pourrait bien sûr définir plusieurs paramètres, du style '192.168.4.1?voyant=vert&servo1=90&servo2=0'

    p += "<script>\n" ;
    p +=  "function addition(val) {\n";                                             
    p +=  "    window.location = window.location.pathname + '?add=' + val ;\n";    
    p +=  "}\n";
    
    p +=  "function allume(val) {\n";                                             
    p +=  "    window.location = window.location.pathname + '?allum=' + val ;\n";    
    p +=  "}\n";

    p += "</script>\n" ;

    // Corps de la page web : affichage de la valeur récupérée sur la carte, et de deux boutons 'ajouter 1' et 'enlever 1'.
    // La fonction addition() définie dans le code javascript ci-dessus, sera appelée lorsqu'on appuie sur ces boutons.

    p += "<body><center>\n" ;
    p += "    </br></br>Valeur actuelle : " + String(brightness) + "</br></br>\n";                 
    p +=  "   <form>\n";  
    p +=  "         <input type='submit' value='+'   formaction='javascript:addition(1);' formmethod=post>\n" ;
    p +=  "         <input type='submit' value='-'   formaction='javascript:addition(-1);' formmethod=post>\n";
    p +=  "         <input type='submit' value='ON'  formaction='javascript:allume(1);' formmethod=post>\n";
    p +=  "         <input type='submit' value='OFF' formaction='javascript:allume(0);' formmethod=post>\n";  
    p +=  "   </form>\n";
    p += "</center></body></html>" ;

    // ça y est, la page web est complètement constituée !
    return p;
  }

/* --------------------------------------------------------------------------------------------------------
 *  runPage01 : gestion de la page web
 *  -------------------------------------------------------------------------------------------------------- */
void runPage01() {
    // Si la page a un paramètre 'add', alors on récupère sa valeur, et on l'ajoute à notre variable 'myValue'.

    if ( myWeb.hasArg("allum")) {
      int myArg = myWeb.arg("allum").toInt() ; 
        if (myArg == 1 )
        {
          //analogWrite(LED, 255);
          brightness = 255;
        }
        else
        {
          brightness = 0;
        }
    }
    
    if ( myWeb.hasArg("add") ) { 
        //Serial.println("Traitement page web, arg = '" + String(myWeb.arg("add")) + "'") ;

        int myArg = myWeb.arg("add").toInt() ; 
        if (myArg == -1 ){
          if (brightness > 0) {
            brightness = brightness + (myArg*10) ;
          }
        }
        if (myArg == +1){
          if (brightness < 255) {
            brightness = brightness + (myArg*10) ;
          }
        }
        //Serial.println("Traitement page web, arg = '" + String(myWeb.arg("add")) + "' --> Nouvelle valeur : " + String(brightness)) ;
    }
    //on actualise la LED
    analogWrite(LedPin, brightness);
    // On renvoie la page Web.
    myWeb.send ( 200, "text/html", webPage01() ); 
    
}


/* --------------------------------------------------------------------------------------------------------
 *  SETUP : Initialisation
 *  -------------------------------------------------------------------------------------------------------- */

void setup() {
  pinMode(LedPin, OUTPUT); // Initialise la broche D2 comme sortie
  digitalWrite(LedPin, LOW);
/*
    // Initialisation de la liaison série, affichage 1er message

    Serial.begin(115200);
    delay(100) ;
    Serial.println(); 
    Serial.println("----------------------") ;
    Serial.println("Exemple de serveur WEB") ;
    Serial.println("----------------------") ;


    // Déclaration du mode "Point d'Accès". On s'arrête là si échec.

    Serial.println("Déclaration Mode AP, SSID \"" + String(mySSID) + "\"") ;
*/
//    if (!WiFi.softAP(mySSID,mySecKey)) {
    if (!WiFi.softAP(mySSID)) {
        Serial.println("Mode AP KO ... :-(") ;
        return ;
    }

    // Affichage de l'adresse IP principale du Point d'Accès.

//    Serial.print("Mode AP OK, IP Address : ") ;
//    Serial.println(WiFi.softAPIP()) ;

    // Définition des points d'entrée du serveur Web (un seul ici), 
    // et démarrage du serveur.

    myWeb.on ( "/", runPage01 );
    myWeb.begin();    

}


/* --------------------------------------------------------------------------------------------------------------
 *  LOOP : fonction appelée régulièrement par le système
 *  ------------------------------------------------------------------------------------------------------------- */

void loop() { 
    // Traitement des requêtes web.
    myWeb.handleClient();
}
