## Grafika L27NCJ Biszterszky Mátyás 

Motoros játék, egy dynamikusan generált útvonalon haladunk, ha nem sikerül az úton maradni vesztettünk, cél hogy minnél tovább jussunk. Az egér oldal irányú mozgatásával tudunk kanyarodni.

Végtelenségig lehet bonyolítani ezért nem fogok minden ötletet megvalósítani viszont itt van pár, ezek közül annyi kerül bele amennyit csak lehet:
  - A motor folyamatosan gyorsul így egyre nehezebb a játék
  - Kamera nézetek váltása/smooth kamera követés
  - (Mozgó fények a motorra)
  - Út menti objektumok, szétszórva
  - Ha túl gyorsan kezdünk kanyarodni elesünk, hisz a motort bedöntéséhez normál esetben idő kell, meg van a dinamikája, nem lehet úgy rángatni mint az egeret.
  
  Ezek főleg ötletek, a felsők valószínűleg bekerülnek a beadandóba
  - Gyorsítás lassítás (itt már nem folyamatosan gyorsaulna, hanem adott átlag sebességet kellene tartani, ha ez lecsökken adott érték alá vesztettünk)
  - Tapadás, elcsúszás. Ha túl nagy erő hat a kerekekre azok csúszni kezdenek tapadást vesztenek és elesünk (vesztünk).
    A fékezés, lassítás és a kanyarodás mértékéből lehet számolni a kerék terhelést.
