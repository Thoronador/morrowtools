===============================================================================
                         Cell Translator für Morrowind
                                 von Thoronador
                                 (08. Mai 2011)
===============================================================================


Zweck des Programmes
====================

Das Programm Cell Translator soll dazu dienen, die Zellnamen eines Plugins für
Morrowind von der englischen in die deutsche Version zu übersetzen. Der umge-
kehrte Weg ist ebenfalls möglich.

Programmversionen
=================

Wenn nichts anderes angegeben ist, so beziehen sich die Angaben in dieser Datei
allesamt auf die Version 0.3_rev238 des Programmes, welche am 08. Mai 2011 er-
stellt wurde. In neueren Programmversionen kann sich die Funktionalität des
Programmes ändern oder neue Funktionen hinzukommen, die hier noch nicht doku-
mentiert sind.


Programmaufruf
==============

Der Aufruf erfolgt über die Kommandozeile, das Programm selbst kann in ein be-
liebiges Verzeichnis entpackt werden und muss nicht im Morrowindverzeichnis
landen. Ein typischer Aufruf sieht so aus:

  cell_translator.exe -f "D:\Spiele\Bethesda\Morrowind\Data Files\Plugin.esp" -o "Neue Datei.esp"

Zu den zwei Pfadangaben: hinter -f muss der Pfad zur zu übersetzenden Plugin-
datei angegeben werden. Im gleichen Verzeichnis wie die angegebene Datei müssen
sich auch die Master-Dateien von Morrowind befinden, sowie die Plugins, von de-
nen das zu übersetzende Plugin abhängig ist. Nach -o kann man den Namen der neu
zu erstellenden .esp-Datei angeben. Diese wird dann im gleichen Verzeichnis er-
stellt, man sollte also Schreibrechte in diesem Verzeichnis haben. Die Angabe
des Namens der neuen Plugindatei kann man auch weglassen, allerdings wird dann
ein vorgegebener Dateiname (out.esp) benutzt. Den Pfad zur zu übersetzenden
Plugindatei muss man zwingend angeben, ohne diese Angabe bricht das Programm ab.

Optional kann man mithilfe des Parameters -xml noch die XML-Datei angeben, in
welcher die Zellnamen und ihre jeweilige Übersetzung angegeben sind. Dies sähe
dann beispielsweise aus wie folgt:

 cell_translator.exe -f "D:\Spiele\Bethesda\Morrowind\Data Files\Plugin.esp" -o "Neue Datei.esp" -xml "Zellnamen.xml"

Gibt man keine XML-Datei an, so wird nach der Datei "cells.xml" im Verzeichnis
von cell_translator.exe gesucht.

Voraussetzung ist natürlich, dass das angegebene Plugins und die XML-Datei auch
existieren. Wird eines der beiden nicht gefunden, bricht das Programm ab.


Parameter
=========

In diesem Abschnitt folgt eine Liste der möglichen Programmparameter für Cell
Translator. Alle Parameter bis auf -f sind optional, das heißt, sie müssen nicht
angegeben werden. Allerdings kann das Fehlen eines Parameters dazu führen, dass
das Programm nicht korrekt ausgeführt werden kann und vor dem Erstellen des
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
                           welche vom Programm übersetzt wird. Dieser Paramater
                           muss immer angegeben werden.
--output NeuesPlugin.esp - legt NeuesPlugin.esp als Name für die neu erstellte
                           Plugindatei fest. Wird dieser Parameter nicht ange-
                           geben, so heißt die Ausgabedatei einfach nur
                           out.esp.
-o NeuesPlugin.esp       - Kurzform von --output
-xml Zellnamen.xml       - legt Zellnamen.xml als die XML-Datei fest, aus
                           welcher die Zellnamen und ihre Übersetzung gelesen
                           werden. Wird dieser Parameter nicht angegeben, so
                           wird die Datei "cells.xml" im Programmverzeichnis
                           verwendet.
--no-scripts             - Wenn dieser Parameter angegeben wird, werden Scripts
                           im Plugin NICHT übersetzt. Dies ist dann anzuraten,
                           wenn Scripte fehlerhaft kompiliert werden, was bei
                           der gegenwärtigen, frühen Programmversion noch häu-
                           figer passieren kann.


Lizenz und Quellcode
====================

Das Programm cell_translator.exe steht unter der GNU General Public Licence 3,
einer freien Softwarelizenz. Der volle Text der Lizenz ist in der Datei GPL.txt
enthalten und kann auch auf http://www.gnu.org/licenses/gpl-3.0.html eingesehen
werden.

Der Quellcode des Programms lässt sich auf Sourceforge.net einsehen, das
Projekt findet sich unter http://sourceforge.net/projects/morrowtools/


Danksagung
==========

Vielen Dank an Bethesda Softworks für The Elder Scrolls III: Morrowind.
Ohne dieses tolle Spiel würde es dieses Programm auch nicht geben.
Mein besonderer Dank gilt Dave Humphrey von UESP.net für seine Übersicht zum
ESM-Dateiformat, welche mir beim Erstellen des Programms eine große Hilfe war.
Jene Übersicht findet sich auf http://www.uesp.net/morrow/tech/mw_esm.txt
wieder.