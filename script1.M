function Main()  
	if($0 == 1)		
		var b := $1;
		if(b->size < 20000)	
			output("name : " + b->name + ", size : " + b->size );
		endif	
		return 0;
	else
		output("wrong usage");
		return 1;
	endif
endfunction

