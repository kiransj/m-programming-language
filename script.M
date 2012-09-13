function testIf()  
	var a := 1;
	while(a < 6)
		if(a < 2)
			output(a+"<2");
		elif(a > 2)
			output(a+">2");
			if(a > 5)	
				output(a + ">5");
			elif(a < 5)
				output(a + "<5");
			else
				output(a + "==5");
			endif
		elif(a == 2)
			output(a + "==2");
		endif
		a := a + 1;
	endwhile
endfunction

function Factorial()  
  #the first argument passed is the value. It can be accessed using $1
  #$0 can be used to access the number of argument passed
	if($1 <= 2)
		return ($1);
	else
		#recursion
		return Factorial($1-1) * $1;
	endif
endfunction

function Fibonacci()
	if($1 < 2) 
		return $1;		
	else
		return Fibonacci($1-1) + Fibonacci($1-2);
	endif
endfunction

function GetKeyValue()
	var  v;
	# Create a KeyValue Object. This is native call the return type is an Object.
	v :=KeyValue();
	#Check if the call of successful. The different return values of
	#typeof() is "int", "string", "float", "object"
	if(typeof(v) == "keyvalue")
		#Add some object to KeyValue.
		KeyValueAdd(v, "version", "10");	
		KeyValueAdd(v, "Path", "/home/kiran/root/bin");	
		KeyValueAdd(v, "time", "10:30");
		KeyValueAdd(v, "LANG", "en");	
		KeyValueAdd(v, "PWD", "~");	
		KeyValueAdd(v, "time", "10:31");
		KeyValueAdd(v, "time", "10:31");
		KeyValueAdd(v, "time", "10:31");
		KeyValueAdd(v, "time", "1:45");
		return v;
	endif
	output("KeyValue() failed");
	return (0);
endfunction

function Main()
	var v;
	testif();
	v := GetKeyValue();	
	if(typeof(v) != "keyvalue")
		output("KeyValue() failed" + typeof(v));
		return (0);
	endif
	Output("KeyValue version : " +KeyValueGet(v, "version"));
	#Print all the entries in the KeyValue Object.
	printKeyValueList(v);

	v := 14;		
	output("Fibonacci("+v+") := " + Fibonacci(v));
	output("Fact(10) = " + Factorial(10));
	return (11);
endfunction

function printKeyValueList()
	#Make sure one argument is passed
	if($0 != 1) 
		output("USAGE: printKeyValueList(keyValueObject)");
		return (0);
	elif(typeof($1) == "keyvalue")
		var v := $1;
		#Setup the iterator
		if(KeyValueIterator(v))
			var a;
			#for each element in the list print it.
			while(typeof(a:=KeyValueNext(v)) == "struct")
				output(a->key+ " = " + a->value);
			endwhile
		else
			output("KeyValueIterator() failed");
		endif
	else
		output("object is of type '" + typeof($1) + "' insted it should be keyvalue");
	endif
	a:=-2;
	output(a);
endfunction	

