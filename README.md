s<style>
.td{
    width: 150px;
    font-size: 30px
}
.table{
    align-items: center;
    align: center;
    vertical-align: center;
}
</style>

<h3> 1. Instructiuni de utilziare</h3>
codul main (de preferat) trebuie rulat astfel:<br>
1. <kbd>sudo -E python main.py</kbd> sau<br>
2. <kbd>sudo -E python main.py ttyUSB0</kbd> <br>
deoarece pe portul de arduino va avea acces 
doar utilizatorul root, iar nimeni altcineva nu
va avea acces asupra citirii datelor
<br>
flagul <kbd>-E</kbd> ajuta pentru a oferi 
utilzatorului root permisiunile de a folosi variabilele locale
ale userului pentru a face rendering la GUI<br>
A doua varianta(2.) se foloseste atunci cand stii portul pe care a fost conectata placuta, iar prima
pentru detectie automata
<br><br>

<b><i>Observatie</i></b> 

<hr>
<h3> 2. Componenta pachetului de date transmis</h3>
pachetul va avea structura de tip:
<ol>
<li><b>RSSI</b> - Received Signal Strength Indicator</li>
<li><b>NR SATT</b> - Numar de sateliti conectati(matematic vb pentru triangulare este
nevoie de 4 pentru a determina un pct in spatiul R<sup>3</sup>, dar pentru acuratete si pentru regesia vectorului
viteza/acceleratie uneori este nevoie de 5 pentru aproximare corecta</li>
<li><b>TIMESTAMP</b> - vom avea ceva de forma SS:MMM, unde S este secunda si M milisecunda, deoarece
ora si minutul lansarii se cunosc</li>
<li><b>LAT</b> - </li>
<li><b>LONG</b> - </li>
<li><b>TEMP</b> - un integer in Celsius, Kelvin ne aduce redundanta cu 
variabile de dimensiuni mult mai mari provenit din diferenta fata de 
un 0 K care este foarte jos fata de Celsius si atunci retinem mult prea
multa dimensiune de date</li>
<li><b>ALT</b> - altitudinea va fi pana in 30 000ft ~ 10km ca marja 
superioara, astfel incat ne putem incadra intr-un uint16_t, echivalentul unui Unicode UTF-16 ca dimensiune</li>
<li><b>ACCEL (X,Y,Z)</b> - aceeleratia va fi pe 3 axe fiecare cu o valoare</li>
<li><b>GYRO (X,Y,Z)</b> - inclinatia va fi de asemenea descompusa pe cele 3 axe</li>
</ol>
Ca si delimitator vom folosi caracterul special '|' (bara verticala) pentru a separa componentele transmisiei ca 
fiind date separate
<hr>
<h3> 3. Tipurile de date folosite</h3>
<table valign="center">

<thead style="font-size: 18px">
<td>Variabila</td>
<td>Tip de data folosit</td>
<td>Exemplu</td>
</thead>

<tr style="height: 70px">
<td align="center">RSSI</td>
<td align="center">uint8_t</td>
<td align="center">30</td>
</tr>

<tr>
<td align="center">SAT</td>
<td align="center">uint8_t</td>
<td align="center">6</td>
</tr>

<tr>
<td align="center">TIMESTAMP</td>
<td align="center">char [6]</td>
<td align="center">20:300<br>24:103<br>25:099</td>
</tr>

<tr>
<td align="center">LAT</td>
<td align="center">uint8_t</td>
<td align="center">2341</td>
</tr>

<tr style="height: 40px">
<td align="center">LONG</td>
<td align="center">uint8_t</td>
<td align="center">4001</td>
</tr>

<tr style="height: 40px">
<td align="center">LONG</td>
<td align="center">uint8_t</td>
<td align="center">4001</td>
</tr>

<tr style="height: 40px">
<td align="center">LONG</td>
<td align="center">uint8_t</td>
<td align="center">4001</td>
</tr>
</table>

