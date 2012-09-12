syntax off
syntax on
syntax case ignore
syntax keyword Type var if endif while endwhile return function endfunction typeof else
syntax region String start=+"+ end=+"+ skip=+\\"+
syntax region Comment start=+#+ end=+\n+ 
"syntax region Number start=+[0-9]+ end=+[^0-9]+ skip=+[^0-9]+ 
"syntax region Argument start=+$[0-9]+ end=+[^0-9]+ 


hi Number term=bold ctermfg=red guifg=red
hi Argument term=bold ctermfg=DarkCyan guifg=DarkCyan

syn match ifstart "\<if.*" 	nextgroup=ifline skipwhite skipempty
syn match ifline  "[^ \t]*"	nextgroup=ifline skipwhite skipempty contained
syn match ifline  "endif"   contained


let b:current_syntax = "M"
