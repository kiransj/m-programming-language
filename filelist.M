#!./a.out
function Main()  
	var obj, tmp;
	obj := FileListObject("/bin");
	if(typeof(obj) != "filelist")
		output("FileListObject() failed");
	else
		var array, idx;
		idx := -1;
		output("FileListObject() succeed");
		while(typeof(tmp := FileListGet(obj))  == "struct")
			if(!tmp->isdir && tmp->executable && (tmp->size > 100000))
				output(tmp->size + "  " + tmp->name);
				array[idx := idx+1] := tmp->name;
			endif
		endwhile
		tmp := -1;
		while(tmp < idx)
			output(array[tmp := tmp+1]);
		endwhile
	endif
	return 0;
endfunction

