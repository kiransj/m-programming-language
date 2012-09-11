function Factorial()  
  #the first argument passed is the value. It can be accessed using $1
  #$0 can be used to access the number of argument passed
  var n := $1; 
	if(n <= 2)
		return (n);
	endif
	return Factorial(n-1) * n;
endfunction

function Fibonacci()
	if($1 < 2) 
		return $1;
	endif
	return Fibonacci($1-1) + Fibonacci($1-2);
endfunction

function GetKeyValue()
	var  v;
	v :=KeyValue();
	if(typeof(v) == "object")
		KeyValueAdd(v, "version", "10");	
		KeyValueAdd(v, "Path", "/home/kiran/root/bin");	
		KeyValueAdd(v, "time", "10:30");
		return v;
	endif
	output("KeyValue() failed");
	return (0);
endfunction

function Main()
	var v, c, d, a;
	v := GetKeyValue();	
	if(typeof(v) != "object")
		output("KeyValue() failed");
		return (0);
	endif
	if(KeyValueIterator(v))
		while(KeyValueNext(v))
			output("Key: "+KeyValueGetKey(v)+ ", value : " + KeyValueGetValue(v));
		endwhile	
	endif
	return (1);
endfunction
