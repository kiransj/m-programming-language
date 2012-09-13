function PrintStruct()
	var a;
	a->count := 10;
	a->num := 3411;
	return (a);
endfunction
function Main()  
	var b := PrintStruct(), a;	
	a := b;
	a->num := b->num - 3000;
	output("type : " + typeof(a), " a->num : " + a->num + " a->count : " + a->count);
	return (11);
endfunction

