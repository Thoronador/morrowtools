===============================================================================
                         Cell Translator für Morrowind
                                 von Thoronador
                                 (26. Juli 2012)
===============================================================================


Zweck des Programmes
====================

Das Programm Cell Translator soll dazu dienen, die Zellnamen eines Plugins für
Morrowind von der englischen in die deutsche Version zu übersetzen, Zellnamen
in Scripts eingeschlossen. Der umgekehrte Weg (deutsch -> englisch) ist eben-
falls möglich.


Programmversionen
=================

Wenn nichts anderes angegeben ist, so beziehen sich die Angaben in dieser Datei
allesamt auf die Version 0.4.1_rev490 des Programmes, welche am 26. Juli 2012
erstellt wurde. In neueren Programmversionen kann sich die Funktionalität des
Programmes ändern oder neue Funktionen hinzukommen, die hier noch nicht doku-
mentiert sind.


Programmaufruf
==============

Der Aufruf erfolgt über die Kommandozeile, das Programm selbst kann in ein be-
liebiges Verzeichnis entpackt werden und muss nicht im Morrowindverzeichnis
landen. Ein typischer Aufruf sieht so aus:

  cell_translator.exe -f "Plugin.esp" -o "Neue Datei.esp" -d "D:\Spiele\Bethesda\Morrowind\Data Files"

Zu den drei Pfadangaben: hinter -f muss der Name der zu übersetzenden Plugin-
datei angegeben werden. Nach -d kann der Pfad zum Data Files-Verzeichnis von
Morrowind angegeben werden. Lässt man diese Angabe weg, wird versucht, das
Verzeichnis aus der Registry auszulesen - was bei installiertem Morrowind auch
immer funktionieren sollte.
Daraus ergibt sich eine verkürzte Aufrufvariante, welche wie folgt aussieht:

  cell_translator.exe -f "Plugin.esp" -o "Neue Datei.esp"

Diese Variante ist in ihrer Auswirkung identisch zum obigen Aufruf, eine intakte
Morrowindinstallation vorausgesetzt.

Nach -o kann man den Namen der neu zu erstellenden .esp-Datei angeben. Diese
wird dann im Data Files-Verzeichnis erstellt, man sollte also Schreibrechte in
diesem Verzeichnis haben. Die Angabe des Namens der neuen Plugindatei kann man
auch weglassen, allerdings wird dann ein vorgegebener Dateiname (out.esp) be-
nutzt. Den Namen der zu übersetzenden Plugindatei (Parameter -f) muss man aber
zwingend angeben, ohne diese Angabe bricht das Programm ab.

Optional kann man mithilfe des Parameters -xml noch die XML-Datei angeben, in
welcher die Zellnamen und ihre jeweilige Übersetzung angegeben sind. Dies sähe
dann beispielsweise aus wie folgt:

 cell_translator.exe -f "Plugin.esp" -o "Neue Datei.esp" -xml "Zellnamen.xml"

Gibt man keine XML-Datei an, so wird nach der Datei "cells.xml" im Verzeichnis
von cell_translator.exe gesucht. (Dies ist der Name der mitgelieferten Datei.)

Voraussetzung ist natürlich, dass das angegebene Plugin und die XML-Datei auch
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
-d DIRECTORY             - legt DIRECTORY als Data Files-Verzeichnis von
                           Morrowind fest. Wird dieser Parameter nicht angege-
                           ben, so wird versucht, das Verzeichnis anhand der
                           Einträge in der Windows-Registry zu ermitteln. Falls
                           dies fehlschlägt, wird ein vordefinierter Wert ge-
                           nutzt.
-dir DIRECTORY           - Alternative zu -d
-f "Plugin.esp"          - legt das Plugin Plugin.esp als die Datei fest, welche
                           vom Programm übersetzt wird. Dieser Paramater muss
                           immer angegeben werden.
--output NeuesPlugin.esp - legt NeuesPlugin.esp als Name für die neu erstellte
                           Plugindatei fest. Wird dieser Parameter nicht ange-
                           geben, so heißt die Ausgabedatei einfach nur
                           out.esp.
-o NeuesPlugin.esp       - Kurzform von --output
--force                  - Normalerweise überschreibt das Programm keine be-
                           stehenden Plugindateien, sodass die Angabe einer ex-
                           istierenden Datei als Ausgabedatei in einem Programm-
                           abbruch resultiert. Falls man dies übergehen und eine
                           bestehende Datei ersetzen möchte, kann man --force
                           verwenden.
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
--dare                   - Mit diesem Parameter wird versucht, Scripte selbst
                           dann zu rekompilieren, wenn diese wahrscheinlich
                           nicht korrekt kompiliert werden können. Das führt
                           einerseits zu einer höheren Erfolgsrate. Andererseits
                           können so erzeugte Dateien auch häufiger Abstürze von
                           Morrowind verursachen, solange man diese Dateien als
                           aktive Plugins nutzt. Benutzung erfolgt also auf
                           eigene Gefahr.


Lizenz, Gewährleistungsausschluss und Quellcode
===============================================

Das Programm cell_translator.exe steht unter der GNU General Public Licence 3,
einer freien Softwarelizenz. Der volle Text der Lizenz ist in der Datei GPL.txt
enthalten und kann auch auf http://www.gnu.org/licenses/gpl-3.0.html eingesehen
werden.

Das Programm wurde in der Hoffnung, dass es nützlich ist, erstellt und verfügbar
gemacht. Das Programm ist nicht vollendet und kann daher Fehler („bugs“) ent-
halten. Aus den genannten Gründen wird es unter dieser Lizenz „so wie es ist“
ohne jegliche Gewährleistung zur Verfügung gestellt. Dies gilt unter anderem
– aber nicht ausschließlich – für Verwendbarkeit für einen bestimmten Zweck,
Mängelfreiheit und Richtigkeit (siehe dazu die entsprechenden Abschnitte der
GNU General Public Licence 3).

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