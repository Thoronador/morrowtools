===============================================================================
                         Cell Translator f�r Morrowind
                                 von Thoronador
                                 (26. Juli 2012)
===============================================================================


Zweck des Programmes
====================

Das Programm Cell Translator soll dazu dienen, die Zellnamen eines Plugins f�r
Morrowind von der englischen in die deutsche Version zu �bersetzen, Zellnamen
in Scripts eingeschlossen. Der umgekehrte Weg (deutsch -> englisch) ist eben-
falls m�glich.


Programmversionen
=================

Wenn nichts anderes angegeben ist, so beziehen sich die Angaben in dieser Datei
allesamt auf die Version 0.4.1_rev490 des Programmes, welche am 26. Juli 2012
erstellt wurde. In neueren Programmversionen kann sich die Funktionalit�t des
Programmes �ndern oder neue Funktionen hinzukommen, die hier noch nicht doku-
mentiert sind.


Programmaufruf
==============

Der Aufruf erfolgt �ber die Kommandozeile, das Programm selbst kann in ein be-
liebiges Verzeichnis entpackt werden und muss nicht im Morrowindverzeichnis
landen. Ein typischer Aufruf sieht so aus:

  cell_translator.exe -f "Plugin.esp" -o "Neue Datei.esp" -d "D:\Spiele\Bethesda\Morrowind\Data Files"

Zu den drei Pfadangaben: hinter -f muss der Name der zu �bersetzenden Plugin-
datei angegeben werden. Nach -d kann der Pfad zum Data Files-Verzeichnis von
Morrowind angegeben werden. L�sst man diese Angabe weg, wird versucht, das
Verzeichnis aus der Registry auszulesen - was bei installiertem Morrowind auch
immer funktionieren sollte.
Daraus ergibt sich eine verk�rzte Aufrufvariante, welche wie folgt aussieht:

  cell_translator.exe -f "Plugin.esp" -o "Neue Datei.esp"

Diese Variante ist in ihrer Auswirkung identisch zum obigen Aufruf, eine intakte
Morrowindinstallation vorausgesetzt.

Nach -o kann man den Namen der neu zu erstellenden .esp-Datei angeben. Diese
wird dann im Data Files-Verzeichnis erstellt, man sollte also Schreibrechte in
diesem Verzeichnis haben. Die Angabe des Namens der neuen Plugindatei kann man
auch weglassen, allerdings wird dann ein vorgegebener Dateiname (out.esp) be-
nutzt. Den Namen der zu �bersetzenden Plugindatei (Parameter -f) muss man aber
zwingend angeben, ohne diese Angabe bricht das Programm ab.

Optional kann man mithilfe des Parameters -xml noch die XML-Datei angeben, in
welcher die Zellnamen und ihre jeweilige �bersetzung angegeben sind. Dies s�he
dann beispielsweise aus wie folgt:

 cell_translator.exe -f "Plugin.esp" -o "Neue Datei.esp" -xml "Zellnamen.xml"

Gibt man keine XML-Datei an, so wird nach der Datei "cells.xml" im Verzeichnis
von cell_translator.exe gesucht. (Dies ist der Name der mitgelieferten Datei.)

Voraussetzung ist nat�rlich, dass das angegebene Plugin und die XML-Datei auch
existieren. Wird eines der beiden nicht gefunden, bricht das Programm ab.


Parameter
=========

In diesem Abschnitt folgt eine Liste der m�glichen Programmparameter f�r Cell
Translator. Alle Parameter bis auf -f sind optional, das hei�t, sie m�ssen nicht
angegeben werden. Allerdings kann das Fehlen eines Parameters dazu f�hren, dass
das Programm nicht korrekt ausgef�hrt werden kann und vor dem Erstellen des
Plugins abbricht.

--help                   - zeigt einen Hilfetext zu den Parametern des Pro-
                           grammes an und beendet das Programm. Alle anderen
                           Parameter werden dann verworfen und es wird keine
                           Plugindatei erstellt.
-?                       - Kurzform von --help
--version                - zeigt die Version des Programmes an und beendet das
                           Programm. Alle anderen Parameter werden dann verwor-
                           fen und es wird keine Plugindatei erstellt.
-d DIRECTORY             - legt DIRECTORY als Data Files-Verzeichnis von
                           Morrowind fest. Wird dieser Parameter nicht angege-
                           ben, so wird versucht, das Verzeichnis anhand der
                           Eintr�ge in der Windows-Registry zu ermitteln. Falls
                           dies fehlschl�gt, wird ein vordefinierter Wert ge-
                           nutzt.
-dir DIRECTORY           - Alternative zu -d
-f "Plugin.esp"          - legt das Plugin Plugin.esp als die Datei fest, welche
                           vom Programm �bersetzt wird. Dieser Paramater muss
                           immer angegeben werden.
--output NeuesPlugin.esp - legt NeuesPlugin.esp als Name f�r die neu erstellte
                           Plugindatei fest. Wird dieser Parameter nicht ange-
                           geben, so hei�t die Ausgabedatei einfach nur
                           out.esp.
-o NeuesPlugin.esp       - Kurzform von --output
--force                  - Normalerweise �berschreibt das Programm keine be-
                           stehenden Plugindateien, sodass die Angabe einer ex-
                           istierenden Datei als Ausgabedatei in einem Programm-
                           abbruch resultiert. Falls man dies �bergehen und eine
                           bestehende Datei ersetzen m�chte, kann man --force
                           verwenden.
-xml Zellnamen.xml       - legt Zellnamen.xml als die XML-Datei fest, aus
                           welcher die Zellnamen und ihre �bersetzung gelesen
                           werden. Wird dieser Parameter nicht angegeben, so
                           wird die Datei "cells.xml" im Programmverzeichnis
                           verwendet.
--no-scripts             - Wenn dieser Parameter angegeben wird, werden Scripts
                           im Plugin NICHT �bersetzt. Dies ist dann anzuraten,
                           wenn Scripte fehlerhaft kompiliert werden, was bei
                           der gegenw�rtigen, fr�hen Programmversion noch h�u-
                           figer passieren kann.
--dare                   - Mit diesem Parameter wird versucht, Scripte selbst
                           dann zu rekompilieren, wenn diese wahrscheinlich
                           nicht korrekt kompiliert werden k�nnen. Das f�hrt
                           einerseits zu einer h�heren Erfolgsrate. Andererseits
                           k�nnen so erzeugte Dateien auch h�ufiger Abst�rze von
                           Morrowind verursachen, solange man diese Dateien als
                           aktive Plugins nutzt. Benutzung erfolgt also auf
                           eigene Gefahr.


Lizenz, Gew�hrleistungsausschluss und Quellcode
===============================================

Das Programm cell_translator.exe steht unter der GNU General Public Licence 3,
einer freien Softwarelizenz. Der volle Text der Lizenz ist in der Datei GPL.txt
enthalten und kann auch auf http://www.gnu.org/licenses/gpl-3.0.html eingesehen
werden.

Das Programm wurde in der Hoffnung, dass es n�tzlich ist, erstellt und verf�gbar
gemacht. Das Programm ist nicht vollendet und kann daher Fehler (�bugs�) ent-
halten. Aus den genannten Gr�nden wird es unter dieser Lizenz �so wie es ist�
ohne jegliche Gew�hrleistung zur Verf�gung gestellt. Dies gilt unter anderem
� aber nicht ausschlie�lich � f�r Verwendbarkeit f�r einen bestimmten Zweck,
M�ngelfreiheit und Richtigkeit (siehe dazu die entsprechenden Abschnitte der
GNU General Public Licence 3).

Der Quellcode des Programms l�sst sich auf Sourceforge.net einsehen, das
Projekt findet sich unter http://sourceforge.net/projects/morrowtools/


Danksagung
==========

Vielen Dank an Bethesda Softworks f�r The Elder Scrolls III: Morrowind.
Ohne dieses tolle Spiel w�rde es dieses Programm auch nicht geben.
Mein besonderer Dank gilt Dave Humphrey von UESP.net f�r seine �bersicht zum
ESM-Dateiformat, welche mir beim Erstellen des Programms eine gro�e Hilfe war.
Jene �bersicht findet sich auf http://www.uesp.net/morrow/tech/mw_esm.txt
wieder.