# FrequencyLetters
Calculateur de fréquence des lettres.

Un programme qui va calculer et aﬀicher la fréquence d’apparition des lettres de l’alphabet dans un ensemble de ﬁchiers textes.
Le programme considère uniquement les lettres ASCII de a à z sans prendre en compte les accents.

Pour accélérer les calculs le programme utilisera un thread pour chaque fichier. Ainsi, le thread principal doit attendre la ﬁn de tous les threads avant d’aﬀicher les fréquences des lettres. Pour éviter des problèmes de synchronisation des calculs à cause des threads le programme utilise des mutexes.
