This program adapts the hash visualization function "fingerprint_randomart"
of OpenSSH into a standalone program designed for a Unix-style command line.

Read about the concept and the original implementation in this interesting
paper: http://www.dirk-loss.de/sshvis/drunken_bishop.pdf

It reads input from stdin and writes to stdout.

It has four optional parameters, each with a mandatory argument. If you use
the defaults, the program should give you the same art that you would get
from OpenSSH.

```
-0, -d	Specify the how input is delimited. Default is newline.
	-0 is null; -d with a parameter for everything else.
	You can specify both and the latter will be used.
-p	Choose a different character set for drawing the art. Should be
	17 characters long. Default is the OpenSSH character set.
-r	Radix (aka numerical base) of input.
	Options are 16 (default) and 64.
-y	Specify size of y-axis of output field, less one. X-axis will
	be derived by doubling this parameter and adding one.
	The axes will always be odd. Default is 8.
```

To build:
```
$ make
```


WARNING: This is a project I gave myself to begin learning C... so use this at your own
risk! And give me feedback!

Screenshots:

```
$ ./randomart <md5_sample
74c8f96ebac915295e34bc47aad812d2
+-----------------+
|                 |
|       . +       |
|        = = .    |
|     . . + *     |
|    . E S B .    |
|     . = = o     |
|      o + +      |
|       o =       |
|        =.       |
+-----------------+
```
```
$ dd if=/dev/urandom bs=1 count=512 | base64 | tr -d '\n' | \
./randomart -r64 -y 32 -p" .,\`;-~*x=#%&@WSE"
512+0 records in
512+0 records out
512 bytes (512 B) copied, 0.00289659 s, 177 kB/s
BekbmfK3Hm0B4guNJ3R2pQoh7oSlv+W4RtCCqLaesJh1re3wSOWZeVPAZDkx6rVilmqqoFob\
7sTkq2/PmVHFje4/ciGx66hJzhY+9/+lmdzsA71RZ855yzu9Wjq34hlYrhdh4tDP8XxJGrvZ\
5MkkE5HKglG9V4S9jM6PFownpi+w94ZB3b++Q9/jUvNvBqAAN2ndfdLSjh2HeUJQPAvKSN4L\
WJhTRDVWJJqoBUnoxtN9mUq315Yqk/TZqgpsCAK3guA5QnQrmHVikV8foBkCgSsNPhLRmhLW\
6OIXME715S02KeREQ6le1cWKm0L7sTKcvcozRXEuIRNT4MWf52HfGOjO4Q/I50WbR5DmpoX6\
GLNy+EyWsu7yCcuzPz0rsJIhF6ogOc1Qud2Hbra3vwKZBuga6R+JG7bvvLtFUglmMVPlYOJF\
XpwGip+IIK3+PSTUNsWvoLd+ERzixiSsWZTiR2z9TbVhtegXrV7RZl06Ub5eJGguvZpXSWbB\
/IPIMIdt0M5LOnYmpaHb/qLuvQX6pb/hZNqn2eShJI3ZIFImDlWVTUqbT7jMPZwopnYUJW6i\
cb8/iBPbROi4NwvI1SOFZBXxwtOpZGNN2ssTio+RhB2hSR93tLr/sdh5/lUcsUfY1PfBtDsd\
59YnAjgxSm+VvCttQiaeUQqiAe3frZqFm0I
+-----------------------------------------------------------------+
|       ..;*-,. `.                                                |
|      ` `-`;-,, ,                                                |
|     , ; ,;~-;.` , .                                             |
|      , ; *,;,- - ;                                              |
|     . , ; ; ; ; ~ ;           .                                 |
|      . , ; - . ; * ` .       . .                                |
|       . . ~ * - - - ` .     , , ,   ,                           |
|          ; - ~ ~ - ` . .     ; ` , ; . .                        |
|           ` ` x ` ` . . .   . . , ; ` ` .                       |
|            - ~ , ~ `   ,         ` ; . ;                        |
|         . , x . ~ ; , . .       , . ` -                         |
|          ; . ` ` * *   ` .     ... ; ~ .                        |
|         . - , , - & E , ; ,,...,; `.; ,                         |
|        . ` ~ ; , W x x . -;;.`,~`-,, .                          |
|     .   , * ; ; = % ~ ;,,,`,``x*=;; ,                           |
| ., ` ` . * - # * - ~ ,,,;`;..;x#~-.* .                          |
|,,.* , ~ ` x @ x . . `, ~`,``,;`S~.~.` , .                       |
|;.= ; ; ` ; = = ` , ., -.,`,-; ` ,-,,.. ,                        |
|,` # % ; , - ; - ~ ,, - `` -` . . ,`,    .                       |
|, * # = ; . ; . * &. - ...`. .   .`,,   . `                      |
| * ` * ~ . - ; ; %,;; ,  ,. ,    ,.- ` . , ,                     |
|` ; ~ ~ ` ` ~ ~ -`=~.- `.. .    . ` ` ` . .                      |
| , - ~ ; , ; `.=,;,~- -.,    .   ` . . . .                       |
|    - , ; . -.*.,. .,;.. .  . . . `   .   .                      |
|   ; . , , `;x ;..  ..,    .   - , , . . ` ,                     |
|  ` , , , ,~;,x ;..,      .   ; = ; . . ` * ,                    |
| ,   ` , `.`-~ ~ ``,.      ` ; - * .     ~ ;                     |
|, . . ;   .,;.~ .,~       ; * ~ - `   . . ,                      |
|.,..., .   .,` ..;.,   . . = *.- ~ , . .                         |
|.; ,,   . ..  ,...x . . , . *`- ~ ~ . , .                        |
|. ; ..   ..  .,. `.= ` .., ..~,`.% ` , , .                       |
| ,...      ..;`;..=.=,.~,   ..x;; ~ , , . ,                      |
|  ,~`      ..,x`.`,=;;-.`.  .`-~,`,, ,.. `.                      |
+-----------------------------------------------------------------+
```
