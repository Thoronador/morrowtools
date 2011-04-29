===============================================================================
                          Spell Renamer für Morrowind
                                von Thoronador
                               (30. April 2011)
===============================================================================


Zweck des Programmes
====================

Das Programm Spell Renamer soll dazu dienen, die Zauber im Zaubermenü von
Morrowind nach Zugehörigkeit zu den Zauberschulen zu sortieren, indem ein ent-
sprechendes Plugin erstellt wird, welches die Zaubernamen ändert. Dazu wird je-
dem Zauber der Name seiner Schule vorangestellt - also B bei Beschwörung, I bei
Illusion, M bei Mystik, V bei Veränderung, W bei Wiederherstellung, Z bei Zer-
störung.

Dadurch entstehen dann Zaubernamen wie beispielsweise:

B Beschworenes Langschwert
I Unsichtbarkeit
M Almsivi Intervention
V Festes Wasser
Z Feuerball

Dies funktioniert auch mit der englischen Version von Morrowind. Dort werden den
Zaubern dann aber entsprechend andere Buchstaben vorangestellt (A bei Alter-
ation, C bei Conjuration, D bei Destruction, I bei Illusion, M bei Mysticism,
R bei Restoration). Prinzipiell sollte das Programm mit jeder Sprachversion von
Morrowind zurechtkommen und die vorangestellten Buchstaben entsprechend der
Sprache der Master- und Plugindateien anpassen, da hierfür die zugehörigen GMST-
Werte ausgelesen werden, aber bisher wurde es nur mit der deutschen und der eng-
lischen Version getestet.


Weitere Buchstabenmuster
========================

In zukünftigen Versionen des Programmes wird es möglicherweise mehrere Buchsta-
benmuster oder Kombinationen aus Buchstaben und Ziffern geben, welche dem Namen
eines Zaubers je nach Wahl vorangestellt werden können. Allerdings sind mir da-
für bisher keine sinnvollen Muster eingefallen. Wer eine Idee dazu hat, kann
sich ja bei mir melden und mir diese mitteilen, vielleicht wird diese dann auch
in das Programm eingebaut. Jedoch sollten diese Kombinationen nicht länger als
drei oder vier Zeichen sein, da es sonst Probleme bei den längeren Zaubernamen
geben könnte.


Programmversionen
=================

Wenn nichts anderes angegeben ist, so beziehen sich die Angaben in dieser Datei
allesamt auf die Version 0.1_rev219 des Programmes, welche am 30.April 2011 er-
stellt wurde. In neueren Programmversionen kann sich die Funktionalität des
Programmes ändern oder neue Funktionen hinzukommen, die hier noch nicht doku-
mentiert sind.


Programmaufruf
==============

Der Aufruf erfolgt über die Kommandozeile, das Programm selbst kann in ein be-
liebiges Verzeichnis entpackt werden und muss nicht im Morrowindverzeichnis
landen. Ein typischer Aufruf sieht so aus:

  spell_rename.exe -d "D:\Spiele\Bethesda\Morrowind\Data Files" -o "Neue Datei.esp"

Zu den zwei Pfadangaben: hinter -d muss der Pfad zum Data Files-Verzeichnis an-
gegeben werden, in dem sich die Master-Dateien von Morrowind befinden. Nach -o
kann man den Namen der neu zu erstellenden .esp-Datei angeben. Diese wird dann
im gleichen Verzeichnis erstellt, man sollte also Schreibrechte in diesem Ver-
zeichnis haben. Beide Angaben kann man auch weglassen, allerdings wird dann ein
Standardverzeichnis (z.Z. C:\Program Files\Bethesda Softworks\Morrowind\
Data Files) und ein vorgegebener Dateiname (out.esp) benutzt, sodass das nicht
auf jedem System funktioniert oder das gewünschte Ergebnis produziert.

Falls man noch weitere Plugins, z.B. Zauberplugins, mit einbeziehen möchte,
lässt sich das nun auch bewerkstelligen, indem man dies zusätzlich nach dem
Parameter -f angibt:

  spell_rename.exe -d "D:\Spiele\Bethesda\Morrowind\Data Files" -o "Neue Datei.esp" -f "Zauberplugin.esp"

Dies kann man auch beliebig oft für weitere Plugins wiederholen.
Folgendes wäre also auch denkbar:

  spell_rename.exe -d "D:\Spiele\Bethesda\Morrowind\Data Files" -o "Neue Datei.esp" -f "Zauberplugin.esp" -f "anderes Plugin.esp" -f "VieleTolleZauber.esp"

Voraussetzung ist natürlich, dass die angegebenen Plugins auch existieren.
Werden die Plugins nicht gefunden, bricht das Programm ab.


Parameter
=========

In diesem Abschnitt folgt eine Liste der möglichen Programmparameter für
Spell Renamer. Alle Parameter sind optional, das heißt, sie müssen nicht ange-
geben werden. Allerdings kann das Fehlen eines Parameters dazu führen, dass das
Programm nicht korrekt ausgeführt werden kann und vor dem Erstellen des Plugins
abbricht. Deshalb sollte man in der Regel mindestens den Pfad zum Data Files-
Verzeichnis von Morrowind angeben, indem man den Parameter -d benutzt.


--data-files VERZEICHNIS  - legt das Verzeichnis VERZEICHNIS als Data Files-
                            Verzeichnis von Morrowind fest. In diesem Verzeich-
                            nis müssen Morrowind.esm und alle andere Plugin-
                            dateien, welche verarbeitet werden sollen, zu finden
                            sein. Existiert das Verzeichnis nicht, wird das
                            Programm vorzeitig abbrechen und keine Plugindatei
                            erstellen.
-d VERZEICHNIS            - Kurzform von --data-files
--output NeuesPlugin.esp  - legt NeuesPlugin.esp als Name für die neu erstellte
                            Plugindatei fest. Wird dieser Parameter nicht ange-
                            geben, so heißt die Ausgabedatei einfach nur
                            out.esp.
-o NeuesPlugin.esp        - Kurzform von --output
--add-files Plugin.esp    - fügt die Datei Plugin.esp zur Liste der zu durchsu-
                            chenden Plugins hinzu. Zauber aus Plugin.esp werden
                            dann ebenfalls mit dem Buchstabenmuster versehen.
                            Dieser Parameter kann wiederholt werden, um so
                            mehrere Plugindateien anzugeben.
-f Plugin.esp             - Kurzform von --add-files
--ini                     - liest die Plugindateien aus dem Abschnitt
                            [Game Files] der Morrowind.ini aus und fügt diese
                            Dateien der Liste der zu durchsuchenden Plugins
                            hinzu. Ist keine Morrowind.ini vorhanden (was bei
                            einer normalen Morrowindinstallation nicht passie-
                            ren sollte), so bricht das Programm ab und erzeugt
                            keine Plugindatei.
-i                        - Kurzform von --ini
--ini-with-discard        - wie --ini, allerdings werden zuvor alle möglicher-
                            weise über den Parameter --add-files bzw. -f hin-
                            zugefügten Plugindateien aus der Pluginliste
                            herausgenommen.
--allow-truncate          - Normalerweise werden diejenigen Zaubernamen vom
                            Programm nicht geändert, welche durch das Hinzu-
                            fügen des Buchstabenmusters der Zauberschule zu
                            lang werden, um Fehler beim Laden des Plugins im
                            Spiel zu vermeiden. Aus diesem Grund kann es aber
                            dazu kommen, dass einige Zauber nicht entsprechend
                            sortiert werden. Durch Angabe dieses Parameters
                            werden solche Zauber auch mit einem Buchstabenmuster
                            versehen, allerdings kann es dann dazu kommen, dass
                            einige Buchstaben am Ende des Zaubernamens abge-
                            schnitten sind, oder dass es beim Laden des erstell-
                            ten Plugins zu Fehlermeldungen kommt.
--verbose                 - zeigt ein paar Informationen zu den verarbeiteten
                            Plugindateien an.
--silent                  - Gegenteil von --verbose; zeigt jene Informationen
                            nicht an.
--help                    - zeigt einen Hilfetext zu den Parametern des Pro-
                            grammes an und beendet das Programm. Alle anderen
                            Parameter werden dann verworfen und es wird keine
                            Plugindatei erstellt.
-?                        - Kurzform von --help
--version                 - zeigt die Version des Programmes an und beendet das
                            Programm. Alle anderen Parameter werden dann ver-
                            worfen und es wird keine Plugindatei erstellt.


Lizenz und Quellcode
====================

Das Programm spell_renamer.exe steht unter der GNU General Public Licence 3,
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