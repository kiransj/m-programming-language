function PrintStruct()
	var a := $1;
	if(typeof($1) == "struct")
		output("num : " + a->num);
		a->count := 3;
	endif
endfunction
function Main()  
	var a;
	a->num := 1100;
	a->count := 1;
	a->temp :=10;
	output("type : " + typeof(a), " a->num : " + a->count + "a->temp : " + a->temp);
endfunction

