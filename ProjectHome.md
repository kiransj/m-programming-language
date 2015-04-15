This code is simple programming language named M.<br>
<br>
<br>

<table><thead><th> <b>feature</b> </th><th> <b>status</b> </th></thead><tbody>
<tr><td> variables </td><td> done </td></tr>
<tr><td> Condition Statments </td><td> done </td></tr>
<tr><td> Loops </td><td> done </td></tr>
<tr><td> Functions</td><td> done </td></tr>
<tr><td> Calling C functions From M </td><td> done </td></tr>
<tr><td> Recursion</td><td> done </td></tr>
<tr><td> Objects  </td><td> done </td></tr>
<tr><td> dynamic structure </td><td> done </td></tr>
<tr><td> Array </td><td> done </td></tr>
<tr><td> Global variables </td><td> Not Yet </td></tr>
<tr><td> Back Trace </td><td> Partially only on Exception </td></tr></tbody></table>

Syntax of M language.<br>
A Sample program for factorial and fibonacci<br>

<br>
<pre><code>function Factorial()  <br>
  #the first argument passed is the value. It can be accessed using $1<br>
  #$0 can be used to access the number of argument passed<br>
	if($1 &lt;= 2)<br>
		return ($1);<br>
	endif<br>
	#recursion<br>
	return Factorial($1-1) * $1;<br>
endfunction<br>
<br>
function Fibonacci()<br>
	if($1 &lt; 2) <br>
		return $1;<br>
	endif<br>
	return Fibonacci($1-1) + Fibonacci($1-2);<br>
endfunction<br>
<br>
#the below program print all the files which are not executable<br>
function Main()<br>
     var obj, tmp;<br>
     obj := FileListObject("/home/kiransj/");<br>
     if(typeof(obj) != "filelist")<br>
         output("FileListObject() failed");<br>
      else<br>
         output("FileListObject() succeed");<br>
          while(typeof(tmp := FileListGet(obj))  == "struct")<br>
               if(!tmp-&gt;isdir &amp;&amp; !tmp-&gt;executable)<br>
                   output(tmp-&gt;name +  "   " );<br>
               endif<br>
          endwhile<br>
      endif<br>
      return 0;<br>
endfunction<br>
<br>
<br>
</code></pre>

The byte code for the above program. To print the byte code compile with -DPRINT_BYTE_CODE macro enabled.<br>
<br>
<pre><code>FUNCTION Factorial<br>
LTE arg(1), int(2), reg(1)<br>
JZ reg(1), 1<br>
RETURN arg(1)<br>
LABEL_1:<br>
SUB arg(1), int(1), reg(2)<br>
PUSH reg(2)<br>
CALL Factorial, 1, reg(3)<br>
MUL reg(3), arg(1), reg(4)<br>
RETURN reg(4)<br>
ENDFUNCTION <br>
FUNCTION Fibonacci<br>
LTH arg(1), int(2), reg(5)<br>
JZ reg(5), 2<br>
RETURN arg(1)<br>
LABEL_2:<br>
SUB arg(1), int(1), reg(6)<br>
PUSH reg(6)<br>
CALL Fibonacci, 1, reg(7)<br>
SUB arg(1), int(2), reg(8)<br>
PUSH reg(8)<br>
CALL Fibonacci, 1, reg(9)<br>
ADD reg(7), reg(9), reg(10)<br>
RETURN reg(10)<br>
ENDFUNCTION <br>
FUNCTION GetKeyValue<br>
VAR var(v), int(0)<br>
CALL KeyValue, 0, reg(1)<br>
MOV var(v), reg(1), reg(2)<br>
PUSH var(v)<br>
CALL typeof, 1, reg(1)<br>
CMP reg(1), string(object), reg(2)<br>
JZ reg(2), 3<br>
PUSH var(v)<br>
PUSH string(version)<br>
PUSH string(10)<br>
CALL KeyValueAdd, 3, reg(3)<br>
PUSH var(v)<br>
PUSH string(Path)<br>
PUSH string(/home/kiran/root/bin)<br>
CALL KeyValueAdd, 3, reg(1)<br>
PUSH var(v)<br>
PUSH string(time)<br>
PUSH string(10:30)<br>
CALL KeyValueAdd, 3, reg(1)<br>
RETURN var(v)<br>
LABEL_3:<br>
PUSH string(KeyValue() failed)<br>
CALL output, 1, reg(1)<br>
RETURN int(0)<br>
ENDFUNCTION <br>
FUNCTION Main<br>
VAR var(v), int(0)<br>
VAR var(c), int(0)<br>
VAR var(d), int(0)<br>
VAR var(a), int(0)<br>
CALL GetKeyValue, 0, reg(1)<br>
MOV var(v), reg(1), reg(2)<br>
PUSH var(v)<br>
CALL typeof, 1, reg(1)<br>
NEQ reg(1), string(object), reg(2)<br>
JZ reg(2), 4<br>
PUSH string(KeyValue() failed)<br>
CALL output, 1, reg(3)<br>
RETURN int(0)<br>
LABEL_4:<br>
PUSH var(v)<br>
PUSH string(version)<br>
CALL KeyValueGet, 2, reg(1)<br>
ADD string(version : ), reg(1), reg(2)<br>
PUSH reg(2)<br>
CALL output, 1, reg(3)<br>
PUSH var(v)<br>
CALL printKeyValueList, 1, reg(1)<br>
RETURN int(11)<br>
ENDFUNCTION <br>
FUNCTION printKeyValueList<br>
NEQ arg(0), int(1), reg(1)<br>
JZ reg(1), 5<br>
PUSH arg(1)<br>
CALL typeof, 1, reg(2)<br>
ADD string(USAGE: printKeyValueList(keyValueObject) : ), reg(2), reg(3)<br>
PUSH reg(3)<br>
CALL output, 1, reg(4)<br>
RETURN int(0)<br>
LABEL_5:<br>
PUSH arg(1)<br>
CALL typeof, 1, reg(1)<br>
NEQ reg(1), string(object), reg(2)<br>
JZ reg(2), 6<br>
PUSH arg(1)<br>
CALL typeof, 1, reg(3)<br>
ADD string(Object passed is not of type object insted it is '), reg(3), reg(4)<br>
ADD reg(4), string('), reg(5)<br>
PUSH reg(5)<br>
CALL output, 1, reg(6)<br>
RETURN int(0)<br>
LABEL_6:<br>
VAR var(v), arg(1)<br>
PUSH var(v)<br>
CALL KeyValueIterator, 1, reg(1)<br>
JZ reg(1), 7<br>
LABEL_8:<br>
PUSH var(v)<br>
CALL KeyValueNext, 1, reg(2)<br>
JZ reg(2), 9<br>
PUSH var(v)<br>
CALL KeyValueGetKey, 1, reg(3)<br>
ADD string(Key: ), reg(3), reg(4)<br>
ADD reg(4), string(, value : ), reg(5)<br>
PUSH var(v)<br>
CALL KeyValueGetValue, 1, reg(6)<br>
ADD reg(5), reg(6), reg(7)<br>
PUSH reg(7)<br>
CALL output, 1, reg(8)<br>
JUMP 8<br>
LABEL_9:<br>
LABEL_7:<br>
ENDFUNCTION <br>
FUNCTION Main1<br>
VAR var(fact), int(10)<br>
ADD string(fact(), var(fact), reg(1)<br>
ADD reg(1), string()=), reg(2)<br>
PUSH var(fact)<br>
CALL Factorial, 1, reg(3)<br>
ADD reg(2), reg(3), reg(4)<br>
PUSH reg(4)<br>
CALL output, 1, reg(5)<br>
ENDFUNCTION <br>
<br>
</code></pre>
<br>
This source code for M has the following parts.<br>
<br>

1. <b>Compiler</b>	<br>
<blockquote>we use the lemon parser to generate the C code from context free grammer.<br>
This parser generates the byte code from the program. The byte code generated<br>
is of register architecture insted of stack architecture.<br>
<br>
What's register architecture.<br>
Consider this statment <br>
<pre><code>	a*(b+c)<br>
</code></pre>
the above statment can be solved in two ways.<br>
the stack architecture<br>
<pre><code>	PUSH A<br>
	PUSH B<br>
	PUSH C<br>
	ADD		=&gt; adds B &amp; C and pushes the result back to stack<br>
	MUL		=&gt; MUL A and (B&amp;C value pushed into stack) and push the result back.<br>
</code></pre>
In the register architecture the same expression becomes<br>
<pre><code>	ADD B, C, %1	=&gt; add B and C and copy it to register 1<br>
	MUL A, %1, %2	=&gt; MUL A and register 1 and copy the result to register 2<br>
</code></pre>
To See the generated bytecode Uncomment the macro in compiler.c file.<br>
The grammer is written for the lemon parsers. (google about lemon parser for more information)<br>
Lemon parser is used in sqlite.<br>
The grammer understands the M lanuage syntax and generates the byteCode. <br>
The grammer is in the file grammer.y<br>
The compiler files are grammer.y, executable.c, functions.c <br>
2. <b>Tokenzier</b> 	 <br>
This is written in flex and c. This program reads a file and then generates a set of tokens <br>
from the file. Examaple<br>
<pre><code>	a:=10*width;<br>
<br>
	the program generates the following tokens&lt;br&gt;<br>
<br>
	variable  &lt;a&gt;<br>
	operator &lt;:=&gt;<br>
	Number	 &lt;10&gt;<br>
	variable &lt;width&gt;<br>
</code></pre>
These are then passed to grammer to check if this syntax is correct<br>
and then byte code are generated from the output of grammer.<br>
See files lexel.l, tokenizer.c files for the tokenizers.<br>
3. <b>Interpreters</b>	 <br>
This interprets the byte code generated by the compiler. <br>
The interpreter uses a set of stacks, linked list to execute the byte code.<br>
The files are execute_context.c<br>
The memory leaks are monitered using valgrind. <br>
Till now there is no memory leaks in the code.<br>