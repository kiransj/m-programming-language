function Factorial()  
  #the first argument passed is the value. It can be accessed using $1
  #$0 can be used to access the number of argument passed
	if($1 <= 2)
		return ($1);
	endif
	#recursion
	return Factorial($1-1) * $1;
endfunction

function Fibonacci()
	if($1 < 2) 
		return $1;
	endif
	return Fibonacci($1-1) + Fibonacci($1-2);
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
		return v;
	endif
	output("KeyValue() failed");
	return (0);
endfunction


function Main()
	var v;
	v := GetKeyValue();	
	if(typeof(v) != "keyvalue")
		output("KeyValue() failed" + typeof(v));
		return (0);
	endif
	Output("KeyValue version : " +KeyValueGet(v, "version"));
	#Print all the entries in the KeyValue Object.
	printKeyValueList(v);
	return (11);
endfunction

function printKeyValueList()
	#Make sure one argument is passed
	if(($0 != 1) || (typeof($1) != "keyvalue"))
		output("USAGE: printKeyValueList(keyValueObject)");
		return (0);
	endif

	var v := $1;
	#Setup the iterator
	if(KeyValueIterator(v))
		#for each element in the list print it.
		while(KeyValueNext(v))
			output("Key: "+KeyValueGetKey(v)+ ", value : " + KeyValueGetValue(v));
		endwhile
	endif
endfunction	

