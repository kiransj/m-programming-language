function Main()  
	var a, i;
	i := 0;
	while(i < 10)	
		a[i]:="kiran" + i;
		i := i + 1;
	endwhile
	
	i := 0;
	while(i < 10)
		output(a[i]);
		i := i + 1;
	endwhile
endfunction

