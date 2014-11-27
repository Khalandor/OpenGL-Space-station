OpenGL-Space-station
====================
Készítsen szimulátort műhold befogáshoz a Мир űrállomás számára!

A fényforrás a távoli **nap**.

Az űrállomás a csillagos égbolt előtt, egy *diffúz*, *procedurálisan textúrázott* **bolygó** körül kering, pályája nem geostracionárius.
A bolygót átlátszó kékes *légkör* veszi körül.

Az **űrállomás** teste *fémes csillogású* forgástest, a sziluett *Catmull-Rom spline*-nal adandó meg.
Az űrállomáson van egy nagyjából kör alakú *lyuk*.
Az űrállomáshoz legalább két *napelem* tábla tartozik.
Az űrállomás egy, a fő tehetetlenségi iránytól különböző tengely körül lassan *forog*.

Az **avatárunk** éppen űrsétát végez, magát az űrállomáshoz kötve egy hosszú gumikötél végén rángatózva.
A gumikötél csak egy adott távolság után *feszül meg*, azon túl *Hooke törvény* szerint nyúlik.
Az űrállomástól nem zérus sebességgel rúgtuk el magunkat.
Avatárunk tömege az űrállomás tömegénél sokkal kisebb.
Az avatár mindig az űrállomás nyílása felé néz.

A **mesterséges hold** gömb alakú, amiből legalább három *henger vagy kúp alakú fúvóka* nyúlik ki (bónusz: ha a fúvókák tengelye nem megy át a súlyponton, akkor a forgását is szabályozhatjuk).
Kezdetben a műhold az űrállomáshoz képest lassan *halad* és *forog*.
A műhold ugyancsak *diffúz-spekuláris*, a fúvókák színe alapján vezérelhetjük őket.

Ha a q, w, ... **billentyűket** lenyomjuk, a betűhöz rendelt fúvókán egy *lángnyelv* tör ki,
a műhold pedig ellentétes irányú I *impulzust* kap.
A lendület és perdület, mint mindig, itt is megmarad.

A játék célja, hogy a mesterséges hold az űrállomás nyílásán besétáljon.

A harmadik feladat a játék "csendélete", 
még nem kell az objektumokat animálni és lángnyelvet sem kell kilőni.
A virtuális világ életre keltése és vezérlése a negyedik házi feladata lesz.

Beadási határidő: 2014. 11. 30. 23:59
