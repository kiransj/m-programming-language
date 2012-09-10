syntax off
syntax on
syntax keyword Type var if endif while endwhile return function endfunction
syntax region String start=+"+ end=+"+ skip=+\\"+
syntax region Comment start=+#+ end=+\n+ 

hi Number term=bold ctermfg=red

let b:current_syntax = "M"
