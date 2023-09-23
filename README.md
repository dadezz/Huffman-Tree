# Huffman-Tree
Semplice implementazione di un albero di huffman
***
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
