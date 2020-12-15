# R-2R ladder Digital to Analog Converter (DAC)
Vojtěch Vyplel; [*Aleš Pokorný*](https://github.com/xpokor79/Digital-electronics-2/tree/master/Labs)

### Cíle projektu
Naprogramovat generátor funkcí v rozsahu frekvencí 10Hz - 5kHz s průběhy: sin, square, ramp a generování signálu DTMF. Informace o aktuálním nastavení se budou zobrazovat na LCD. Zadávání hodnot bude realizováno pomocí klávesnice 4x4.

## Popis hardware
Základem našeho generátoru funkcí bude mikroprocesor atmega328p a D/A převodík typu R2R postavený z odporové sítě.
#### R2R D/A převodník
U tohoto typu převodníku se vstupní bitstream převádí na výstupní napětí pomocí rezistorové sítě. Převodník podle hodnoty bitstreamu přiřazuje jednotlivým odporovým sítím napětí 0V nebo Vref. Výstupní napětí je poté rovno součtem všech napětí na jednotlivých rezistorových sítích. Jednotlivé hodnoty rezitorů se stanovují pomocí 
pravidla R2R, zálkadní rozsah hodnot rezistorů zvolili hodnoty RA = 1000 ohm RB = 2000 ohm.. Jako referenční napětí je stanoveno plné napětí na výstupu z mikroprocesoru tj. 5V. Výstupní napětí z převodníku je lineárně závislé na vstupních datech. 

&nbsp;
Výstupní napětí můžeme vypočítat podle rovnice:
&nbsp;

<img src = "https://github.com/Vojvyp/Digital-electronics-2/blob/master/Labs/project/pictures/CodeCogsEqn4.gif">

&nbsp;
Kde Vbit = 0/5 v závislosti na bit = 0/1. 
&nbsp;

Nejmenší napětí které můžeme převodníkem vytvořit je dáno rovnicí:
&nbsp;

&nbsp;
<img src = "https://github.com/Vojvyp/Digital-electronics-2/blob/master/Labs/project/pictures/CodeCogsEqn.gif">
&nbsp;

&nbsp;
<img src = "https://github.com/Vojvyp/Digital-electronics-2/blob/master/Labs/project/pictures/CodeCogsEqn2.gif">
&nbsp;

Maximální napětí je: 5V - 0.019V = 4.9804V
&nbsp;

&nbsp;
Teoretické schema
&nbsp;

<img src = "https://github.com/Vojvyp/Digital-electronics-2/blob/master/Labs/project/pictures/bitteor.png">

&nbsp;

&nbsp;
Schéma zapojení reálného převodníku
&nbsp;

&nbsp;
<img src = "https://github.com/Vojvyp/Digital-electronics-2/blob/master/Labs/project/pictures/dacsim.png">

&nbsp;

&nbsp;
<img src = "https://github.com/Vojvyp/Digital-electronics-2/blob/master/Labs/project/pictures/schema3.png">
&nbsp;

&nbsp;

#### 4x4 klávesnice
Klávesnice tvoří matici. Při stisku tlačítka se přenese informace o stisku jak na pinech pro řádky. klávesnice je zapojena s děliči napětí. Pro každé tlačítko se objeví na vstupu A/D převodníku v mikrokontroléru určité napětí. 
&nbsp;

&nbsp;
Význam tlačítek na klávesnici:
&nbsp;

&nbsp;
S - sinus
&nbsp;

&nbsp;
R - pila
&nbsp;

&nbsp;
Q - obdélník
&nbsp;

&nbsp;
D - DTMF
&nbsp;

&nbsp;
(*) - delete - nastavení výchozí hodnoty 10Hz
&nbsp;

&nbsp;
(#)- enter
&nbsp;

&nbsp;
číslice - zadání frekvence v Hz/ DTMF: číslicím odpovídají frekvence DTMF.
&nbsp;

#### LCD (Liquid Crystal Display)
Pro ovládání LCD použijeme 4 bitový mód, díky čemuž minimaluzijeme počet potřebných pinů.(využití knihovny ze cvičení)


&nbsp;
## Popis kódu a simulace

&nbsp;
Vývojový diagram programu:
&nbsp;

<img src = "https://github.com/Vojvyp/Digital-electronics-2/blob/master/Labs/project/pictures/vyvojdiag.png">

Displej:

Na displeji je zobrazován režim, ve kterém generátor běží a také frekvence zvoleného signálu, případně zvolené tlačítko pro DTMF.
&nbsp;

&nbsp;
<img src = "https://github.com/Vojvyp/Digital-electronics-2/blob/master/Labs/project/pictures/display.png">
&nbsp;

&nbsp;
Kód začíná tím, že se přednastaví displej (knihovna lcd), nastaví se piny potřebné k DA převodníku na výstupní ([*Vlastní knihovna*](https://github.com/Vojvyp/Digital-electronics-2/blob/master/Labs/project/project/project/project/gen_functions.c), která využívá knihovnu GPIO) a nastavení AD převodníku pro čtení stisknutého tlačítka z klávesnice.
Dále jsou nastavovány 2 časovače. Časovač Timer/Counter1, který se stará o vstupy a Timer/Counter0 pro výstupy (knihovna timer). 
Po zapnutí je generátor v výchozím stavu - SIN 10 Hz

Časovač1: 

Časovač 1 načítá pomocí AD převodníku hodnotu napětí, díky které kód vyhodnocuje zmáčknuté tlačítko. Dále také vypisuje hodnotu frekvence na displej včetně jednotky a mění text zvoleného módu.
Po stisknutí je do proměnné pro frekvenci v podobě textu přidána číslice, případně se změní proměnná pro generovanou fukci a změní se proměnná pro určování pozice .
Výjimkou jsou tlačítka * a #. Tlačítko '*' provádí mazaní textové proměnné pro frekvenci a zobratuje výchozí hodnotu (10 Hz), avšak pozici pro psaní posouvá opět na začátek řetězce.
Pomocí tlačítka '#' se textová proměnná pro frekvenci převede na číslo a uloží do příslušné proměnné. Díky tomu se provede přeladění funkce. 
V případě, že by zadaná frekvence nebyla v povoleném rozsahu (10 až 5k Hz) se na displej zobrazí hláška error na místo frekvence. Krom toho se opět přesune pozice na zapisování na začátek.
Na konci kódu časovače 1 se opět pouští AD převod. Časovač přeteče každých 262 ms, což by měla být dostatečná doba pro AD převodník, aby dokončil svou činnost.

[*Odkaz na kód*](https://github.com/Vojvyp/Digital-electronics-2/blob/master/Labs/project/project/project/project/main.c)

Časovač0:

Tento časovač přetéká každých 16 us, aby bylo dosaženo co největší možné frekvence. V tomto časovači se podle zvoleného režimu (módu) generují jednotlivé průběhy.
Na začátku se napočítá hodnota pro velikost kroku změny, která je použita pro funkce. Je-li zvolena funkce sinus, přiřazují se na výstup hodnoty z look-up tabulky a aktualizuje se pozice hodnot podle velikosti změny. 
Fukce ramp zvyšuje výstupní hodnotu napětí převodníku o velikost kroku dokud nedojde k tomu, že by byla hodnota větší než maximální možná, v takové případě dojde k vynulování výstupu.
Fukce pro obdélník si spočítá kolik je možno udělat změn pro danou frekvenci a pomocí časovače půlku z této hodnoty pro změny je výstup na hodnotě 0 a druhou polovinu na hodnotě maximální.
V režimu DTMF dochází nejprve k určení frekvencí pro zvolené tlačítko. Poté se spočítá velikost kroku v tabulce pro jednotlivé frekvence a výsledná hodnota výstupu je dána součtem těchto 2 hodnot podělené 2.
Časovač se měl také starat o vypisování informací pomocí knihovny uart, avšak po přidání kódu pro uart celá aplikace zamrzne. 

[*Odkaz na kód*](https://github.com/Vojvyp/Digital-electronics-2/blob/master/Labs/project/project/project/project/main.c)


[*Vlastní knihovna:*](https://github.com/Vojvyp/Digital-electronics-2/blob/master/Labs/project/project/project/project/gen_functions.c)

Knihovna obsahuje 3 funkce: output_init, output_bistream a freq_to_stepsize,

funkce ouptut_init nemá žádné vstupní ani výstupní parametry. Slouží k nastavení definovaných pinů na výstupní.

fukce ouput_bistream má vstupní parametr - 8 bitové slovo, díky kterému se nastavuje výstupní napětí DA převodníku.
Funkce prochází slovo bit po bitu a zapisuje jednotlivé bity na příšlušné piny.

funkce freq_to_stepsize má vstupní i výstupní parametry. Vstupem je frekvence a výstup tvoří velikost kroku změny.
fukce je používáná pro generování DTMF.

## Simulace
&nbsp;
Simulace režimu generování sinusového signálu:
&nbsp;

&nbsp;
<img src = "https://github.com/Vojvyp/Digital-electronics-2/blob/master/Labs/project/pictures/SIN%2010Hz.png">
&nbsp;
Simulace režimu generování signálu pila:
&nbsp;


&nbsp;
<img src = "https://github.com/Vojvyp/Digital-electronics-2/blob/master/Labs/project/pictures/RAMP%2010%20Hz.png">
&nbsp;

&nbsp;
Simulace režimu generování obdelníkového signálu:
&nbsp;


&nbsp;
<img src = "https://github.com/Vojvyp/Digital-electronics-2/blob/master/Labs/project/pictures/SQR%2020Hz.png">
&nbsp;

&nbsp;
Simulace režimu generování signálu DTMF:
&nbsp;


&nbsp;
<img src = "https://github.com/Vojvyp/Digital-electronics-2/blob/master/Labs/project/pictures/DTMF%201.png">
&nbsp;

## Závěr
Vytvořený generátor umí generovat tři základní funkce (sinus, pila, obdélník) v rozsahu frekvencí 10-5000 Hz. Dále generuje DTMF signál pro talčítka 0-9 včetně * #. Generátor měl také odesílat informace o zvoleném režimu, frekvenci a zvoleném tlačítku DTMF. To se nám ovšem nepodařilo zprovoznit, aplikace zamrzne. V simulátoru občas dojde k odchylkám signálů, ale to je pravděpodobně způsobeno simulátorem. 
&nbsp;


&nbsp;
## Reference
&nbsp;

&nbsp;
https://2n3904blog.com/r2r-dac-dc-simulation/
&nbsp;

&nbsp;
https://2n3904blog.com/r2r-dac/
&nbsp;

&nbsp;
https://www.electronicwings.com/arduino/4x4-keypad-interfacing-with-arduino-uno

