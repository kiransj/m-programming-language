function Main()  
	var n := 10;
	var fact := 1;

	while(n != 0)
		fact := fact * n;
		n := n -1;
	endwhile
	output("result " + fact);
	return (fact);
endfunction

function Main1()
	var n:= 1;
	var fact := 1;
	while(n < 10)
		fact := fact * n;
		n := n+1;
	endwhile
	output("hello world " + fact);
endfunction	
