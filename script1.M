function Main()  
	var obj;
	obj := FileListObject(".");
	if(typeof(obj) != "filelist")
		output("FileListObject() failed");
	else
		output("FileListObject() succeed");
		var tmp;
		while(typeof(tmp := FileListGet(obj))  == "struct")
			if(tmp->isdir)
				output(tmp->name +  "   " );
			endif
		endwhile
	endif
	return 0;
endfunction

