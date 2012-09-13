function Main()  
	var obj;
	obj := FileListObject("/bin");
	obj := FileListObject("/bin");
	obj := FileListObject("/bin");
	obj := FileListObject("/bin");
	obj := FileListObject("/bin");
	if(typeof(obj) != "filelist")
		output("FileListObject() failed");
	else
		output("FileListObject() succeed");
		var tmp;
		while(typeof(tmp := FileListGet(obj))  == "struct")
			output(tmp->name +  "   " + tmp->size);
		endwhile
		tmp := FileListGet(obj);
		tmp := FileListGet(obj);
		tmp := FileListGet(obj);
		tmp := FileListGet(obj);
		tmp := FileListGet(obj);
		output("tmp := " + tmp);
	endif
	return 0;
endfunction

