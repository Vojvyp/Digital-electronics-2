# R-2R ladder Digital to Analog Converter (DAC)
Vojtěch Vyplel; [*Aleš Pokorný*](https://github.com/xpokor79/Digital-electronics-2/tree/master/Labs)

### Project objectives
Naprogramovat generátor funkcí v rozsahu frekvencí 10Hz - 5kHz s průběhy: sin, square, ramp a generování signálu DTMF. Informace o aktuálním nastavení se budou zobrazovat na LCD. Zadávání hodnot bude realizováno pomocí klávesnice 4x4.Generátor bude možné ovládat pomocí UART.

## Hardware description
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
<img src = "https://github.com/Vojvyp/Digital-electronics-2/blob/master/Labs/project/pictures/schema2.png">
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
Q - obdelník
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
Pro ovládání LCD použijeme 4 bitový mód, díky čemuž minimaluzijeme počet potřebných pinů.

## Code description and simulations

&nbsp;
Vývojový diagram programu:
&nbsp;

<img src = "https://github.com/Vojvyp/Digital-electronics-2/blob/master/Labs/project/pictures/diagram.png">

## Video/Animation

*Write your text here*


## References

*Write your text here*
