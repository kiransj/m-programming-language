function Main()  
	var obj, tmp;
	obj := FileListObject("/home/kiransj/");
	if(typeof(obj) != "filelist")
		output("FileListObject() failed");
	else
		output("FileListObject() succeed");
		while(typeof(tmp := FileListGet(obj))  == "struct")
			if(!tmp->isdir && !tmp->executable)
				output(tmp->name +  "   " );
			endif
		endwhile
	endif
	return 0;
endfunction

