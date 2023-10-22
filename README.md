# Compressione di Huffman
Semplice implementazione di un algoritmo di compressione testi basato sull'albero di huffman. 

Diviso in 3 parti
 - [x] creazione dell'albero (vedasi sezione "huffman-Tree")
 - [x] Compressione (vedasi i commenti nel codice)
 - [ ] Decompressione (in fase di debugging: va in loop infinito (creando file di interi gb con la stessa lettera ripetuta))

***

## Huffman-Tree
[da Wikipedia]
La codifica di Huffman usa un metodo specifico per scegliere la rappresentazione di ciascun simbolo, risultando in un codice senza prefissi (cioè in cui nessuna stringa binaria di nessun simbolo è il prefisso della stringa binaria di nessun altro simbolo) che esprime il carattere più frequente nella maniera più breve possibile. È stato dimostrato che la codifica di Huffman è il più efficiente sistema di compressione di questo tipo: nessun'altra mappatura di simboli in stringhe binarie può produrre un risultato più breve nel caso in cui le frequenze di simboli effettive corrispondono a quelle usate per creare il codice.

Per un insieme di simboli la cui cardinalità è una potenza di due e con una distribuzione probabilistica uniforme, la codifica di Huffman equivale alla semplice codifica a blocchi binari. 

### Implementazione base
Questa tecnica funziona creando un albero binario di simboli:
1. Ordina i simboli, in una lista, in base al conteggio delle loro occorrenze.
2. Ripeti i seguenti passi finché la lista non contiene un unico simbolo:
    * Prendi dalla lista i due simboli con la frequenza di conteggio minore. Crea un albero di Huffman che ha come "figli" questi due elementi, e crea un nodo "genitore"
    * Assegna la somma del conteggio delle frequenze dei figli al genitore e ponilo nella lista in modo da mantenere l'ordine.
    * Cancella i figli dalla lista.
3. Assegna una parola codice a ogni elemento basandosi sul path a partire dalla radice.

***

## Compressione
Una volta creato l'albero, ai rami destra e sinistra di ogni nodo si decide che numero tra 0 e 1 associare. Per ogni lettera del testo da comprimere, si parte dalla radice e si arriva alla foglia corrispondente, scrivendo in memoria un bit per ogni scelta di ramo.
Poniamo per esempio di avere un albero così costruito: `((W,(T,Q)),S)`. Associamo al ramo Sinistro lo 0, a quello destro l'1. 
la codifica sarà quindi: `{'W'='00', 'T'='010', 'Q'='011', 'S'='1'}`. Si noti che nessuna codifica è prefisso di un'altra.

Il testo 'SWWSSSTQ' in memoria viene normalmente salvato così (ASCII): `01010011 01010111 01010111 01010011 01010011 01010011 01010100 01010001`.
Grazie a questo algoritmo di compressione, potrà ora essere salvato come `1 00 00 1 1 1 010 011`. Suddividendo in byte, `10000111 01001100`, con un risparmio di 6 byte su 8.

Ovviamente a inizio testo va aggiunto l'albero (in ascii), quindi `((W,(T,Q)),S)`. Ma per input molto grandi, lo spazio da esso occupato è un male trascurabile rispetto al beneficio portato dalla compressione.

***

## Decompressione
La decompressione avviene con il processo opposto rispetto alla compressione. Inizia con il parsing dell'albero binario: dalla stringa a parentesi bilanciate ricrea la sruttura dati. Prosegue poi con la lettura di bit: per ogni bit segue il ramo corrispondente fino ad arrivare a una foglia e quindi a una lettera da porre nel file decompresso.
