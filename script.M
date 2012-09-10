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
	v :=KeyValueInit();
	if(typeof(v) == "object")
		KeyValueAdd(v, "CreatedBy", "GetKeyValue");	
		return v;
	endif
	output("KeyValueInit() failed");
	return (0);
endfunction

function Main()
	var v, c, d, a;
	v := GetKeyValue();	
	KeyValueAdd(v, "version", 7);	
	KeyValueAdd(v, "time", "10:30");	
	output("version : " + KeyValueGet(v, "version"));
	output("time : " + KeyValueGet(v, "time"));
	output("CreatedBy : " + KeyValueGet(v, "CreatedBy"));
endfunction
