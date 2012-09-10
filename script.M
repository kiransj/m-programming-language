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

function Main()
	var ans, value:=10, out;

	value := 1;
	while(value < 24)
		ans := Fibonacci(value);
		Output("Fibo(" +  value +  ") := " +  ans);
		value := value + 1;
	endwhile
	
	value := 10;
	out := "Fact("+value+") := " + Factorial(value);
	Output(out);
	out := 0;
	return ("done");
endfunction
