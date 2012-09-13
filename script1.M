function PrintStruct()
	var a;
	a->count := 10;
	a->num := 3411;
	return (a);
endfunction
function Main()  
	var b := $1;
	if(b->size > 20000)	
		output("name : " + b->name + ", size : " + b->size);
	endif		
endfunction

