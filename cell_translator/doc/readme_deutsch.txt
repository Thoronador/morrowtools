===============================================================================
                         Cell Translator f�r Morrowind
                                 von Thoronador
                                 (08. Mai 2011)
===============================================================================


Zweck des Programmes
====================

Das Programm Cell Translator soll dazu dienen, die Zellnamen eines Plugins f�r
Morrowind von der englischen in die deutsche Version zu �bersetzen. Der umge-
kehrte Weg ist ebenfalls m�glich.

Programmversionen
=================

Wenn nichts anderes angegeben ist, so beziehen sich die Angaben in dieser Datei
allesamt auf die Version 0.3_rev238 des Programmes, welche am 08. Mai 2011 er-
stellt wurde. In neueren Programmversionen kann sich die Funktionalit�t des
Programmes �ndern oder neue Funktionen hinzukommen, die hier noch nicht doku-
mentiert sind.


Programmaufruf
==============

Der Aufruf erfolgt �ber die Kommandozeile, das Programm selbst kann in ein be-
liebiges Verzeichnis entpackt werden und muss nicht im Morrowindverzeichnis
landen. Ein typischer Aufruf sieht so aus:

  cell_translator.exe -f "D:\Spiele\Bethesda\Morrowind\Data Files\Plugin.esp" -o "Neue Datei.esp"

Zu den zwei Pfadangaben: hinter -f muss der Pfad zur zu �bersetzenden Plugin-
datei angegeben werden. Im gleichen Verzeichnis wie die angegebene Datei m�ssen
sich auch die Master-Dateien von Morrowind befinden, sowie die Plugins, von de-
nen das zu �bersetzende Plugin abh�ngig ist. Nach -o kann man den Namen der neu
zu erstellenden .esp-Datei angeben. Diese wird dann im gleichen Verzeichnis er-
stellt, man sollte also Schreibrechte in diesem Verzeichnis haben. Die Angabe
des Namens der neuen Plugindatei kann man auch weglassen, allerdings wird dann
ein vorgegebener Dateiname (out.esp) benutzt. Den Pfad zur zu �bersetzenden
Plugindatei muss man zwingend angeben, ohne diese Angabe bricht das Programm ab.

Optional kann man mithilfe des Parameters -xml noch die XML-Datei angeben, in
welcher die Zellnamen und ihre jeweilige �bersetzung angegeben sind. Dies s�he
dann beispielsweise aus wie folgt:

 cell_translator.exe -f "D:\Spiele\Bethesda\Morrowind\Data Files\Plugin.esp" -o "Neue Datei.esp" -xml "Zellnamen.xml"

Gibt man keine XML-Datei an, so wird nach der Datei "cells.xml" im Verzeichnis
von cell_translator.exe gesucht.

Voraussetzung ist nat�rlich, dass das angegebene Plugins und die XML-Datei auch
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
-f "C:\MW\Plugin.esp"    - legt das Plugin C:\MW\Plugin.esp als die Datei fest,
                           welche vom Programm �bersetzt wird. Dieser Paramater
                           muss immer angegeben werden.
--output NeuesPlugin.esp - legt NeuesPlugin.esp als Name f�r die neu erstellte
                           Plugindatei fest. Wird dieser Parameter nicht ange-
                           geben, so hei�t die Ausgabedatei einfach nur
                           out.esp.
-o NeuesPlugin.esp       - Kurzform von --output
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


Lizenz und Quellcode
====================

Das Programm cell_translator.exe steht unter der GNU General Public Licence 3,
einer freien Softwarelizenz. Der volle Text der Lizenz ist in der Datei GPL.txt
enthalten und kann auch auf http://www.gnu.org/licenses/gpl-3.0.html eingesehen
werden.

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