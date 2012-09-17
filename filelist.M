#!./a.out
function Main()  
	var obj, tmp;
	obj := FileListObject("/bin");
	if(typeof(obj) != "filelist")
		output("FileListObject() failed");
	else
		output("FileListObject() succeed");
		while(typeof(tmp := FileListGet(obj))  == "struct")
			if(!tmp->isdir && tmp->executable && (tmp->size > 100000))
				output(tmp->size + "  " + tmp->name);
			endif
		endwhile
	endif
	return 0;
endfunction

