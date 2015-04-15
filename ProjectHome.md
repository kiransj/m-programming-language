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
<pre><code>function Factorial()  
  #the first argument passed is the value. It can be accessed using $1
  #$0 can be used to access the number of argument passed
	if($1 &lt;= 2)
		return ($1);
	endif
	#recursion
	return Factorial($1-1) * $1;
endfunction

function Fibonacci()
	if($1 &lt; 2) 
		return $1;
	endif
	return Fibonacci($1-1) + Fibonacci($1-2);
endfunction

#the below program print all the files which are not executable
function Main()
     var obj, tmp;
     obj := FileListObject("/home/kiransj/");
     if(typeof(obj) != "filelist")
         output("FileListObject() failed");
      else
         output("FileListObject() succeed");
          while(typeof(tmp := FileListGet(obj))  == "struct")
               if(!tmp-&gt;isdir &amp;&amp; !tmp-&gt;executable)
                   output(tmp-&gt;name +  "   " );
               endif
          endwhile
      endif
      return 0;
endfunction


</code></pre>

The byte code for the above program. To print the byte code compile with -DPRINT_BYTE_CODE macro enabled.<br>
<br>
<pre><code>FUNCTION Factorial
LTE arg(1), int(2), reg(1)
JZ reg(1), 1
RETURN arg(1)
LABEL_1:
SUB arg(1), int(1), reg(2)
PUSH reg(2)
CALL Factorial, 1, reg(3)
MUL reg(3), arg(1), reg(4)
RETURN reg(4)
ENDFUNCTION 
FUNCTION Fibonacci
LTH arg(1), int(2), reg(5)
JZ reg(5), 2
RETURN arg(1)
LABEL_2:
SUB arg(1), int(1), reg(6)
PUSH reg(6)
CALL Fibonacci, 1, reg(7)
SUB arg(1), int(2), reg(8)
PUSH reg(8)
CALL Fibonacci, 1, reg(9)
ADD reg(7), reg(9), reg(10)
RETURN reg(10)
ENDFUNCTION 
FUNCTION GetKeyValue
VAR var(v), int(0)
CALL KeyValue, 0, reg(1)
MOV var(v), reg(1), reg(2)
PUSH var(v)
CALL typeof, 1, reg(1)
CMP reg(1), string(object), reg(2)
JZ reg(2), 3
PUSH var(v)
PUSH string(version)
PUSH string(10)
CALL KeyValueAdd, 3, reg(3)
PUSH var(v)
PUSH string(Path)
PUSH string(/home/kiran/root/bin)
CALL KeyValueAdd, 3, reg(1)
PUSH var(v)
PUSH string(time)
PUSH string(10:30)
CALL KeyValueAdd, 3, reg(1)
RETURN var(v)
LABEL_3:
PUSH string(KeyValue() failed)
CALL output, 1, reg(1)
RETURN int(0)
ENDFUNCTION 
FUNCTION Main
VAR var(v), int(0)
VAR var(c), int(0)
VAR var(d), int(0)
VAR var(a), int(0)
CALL GetKeyValue, 0, reg(1)
MOV var(v), reg(1), reg(2)
PUSH var(v)
CALL typeof, 1, reg(1)
NEQ reg(1), string(object), reg(2)
JZ reg(2), 4
PUSH string(KeyValue() failed)
CALL output, 1, reg(3)
RETURN int(0)
LABEL_4:
PUSH var(v)
PUSH string(version)
CALL KeyValueGet, 2, reg(1)
ADD string(version : ), reg(1), reg(2)
PUSH reg(2)
CALL output, 1, reg(3)
PUSH var(v)
CALL printKeyValueList, 1, reg(1)
RETURN int(11)
ENDFUNCTION 
FUNCTION printKeyValueList
NEQ arg(0), int(1), reg(1)
JZ reg(1), 5
PUSH arg(1)
CALL typeof, 1, reg(2)
ADD string(USAGE: printKeyValueList(keyValueObject) : ), reg(2), reg(3)
PUSH reg(3)
CALL output, 1, reg(4)
RETURN int(0)
LABEL_5:
PUSH arg(1)
CALL typeof, 1, reg(1)
NEQ reg(1), string(object), reg(2)
JZ reg(2), 6
PUSH arg(1)
CALL typeof, 1, reg(3)
ADD string(Object passed is not of type object insted it is '), reg(3), reg(4)
ADD reg(4), string('), reg(5)
PUSH reg(5)
CALL output, 1, reg(6)
RETURN int(0)
LABEL_6:
VAR var(v), arg(1)
PUSH var(v)
CALL KeyValueIterator, 1, reg(1)
JZ reg(1), 7
LABEL_8:
PUSH var(v)
CALL KeyValueNext, 1, reg(2)
JZ reg(2), 9
PUSH var(v)
CALL KeyValueGetKey, 1, reg(3)
ADD string(Key: ), reg(3), reg(4)
ADD reg(4), string(, value : ), reg(5)
PUSH var(v)
CALL KeyValueGetValue, 1, reg(6)
ADD reg(5), reg(6), reg(7)
PUSH reg(7)
CALL output, 1, reg(8)
JUMP 8
LABEL_9:
LABEL_7:
ENDFUNCTION 
FUNCTION Main1
VAR var(fact), int(10)
ADD string(fact(), var(fact), reg(1)
ADD reg(1), string()=), reg(2)
PUSH var(fact)
CALL Factorial, 1, reg(3)
ADD reg(2), reg(3), reg(4)
PUSH reg(4)
CALL output, 1, reg(5)
ENDFUNCTION 

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
<pre><code>	a*(b+c)
</code></pre>
the above statment can be solved in two ways.<br>
the stack architecture<br>
<pre><code>	PUSH A
	PUSH B
	PUSH C
	ADD		=&gt; adds B &amp; C and pushes the result back to stack
	MUL		=&gt; MUL A and (B&amp;C value pushed into stack) and push the result back.
</code></pre>
In the register architecture the same expression becomes<br>
<pre><code>	ADD B, C, %1	=&gt; add B and C and copy it to register 1
	MUL A, %1, %2	=&gt; MUL A and register 1 and copy the result to register 2
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
<pre><code>	a:=10*width;

	the program generates the following tokens&lt;br&gt;

	variable  &lt;a&gt;
	operator &lt;:=&gt;
	Number	 &lt;10&gt;
	variable &lt;width&gt;
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