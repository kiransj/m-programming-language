function Main()  
	var a := 1;
	while(a < 6)
		if(a < 2)
			output("a<2");
		elif(a > 2)
			output("a>2");
			if(a > 5)	
				output("a>5");
			elif(a < 5)
				output("a<5)");
			else
				output("a==5");
			endif
		elif(a == 2)
			output("a==2");
		endif
		a := a + 1;
	endwhile
endfunction

