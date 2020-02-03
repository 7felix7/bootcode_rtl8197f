""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

" display related  

""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

"set shortmess=atI   " do not show after startup  

"winpos 5 5          " set window location  

"set lines=40 columns=155    " set window size

"set nu              " show line number

set go=             " do not show graph button

"color asmanian2     " set background 

set guifont=Courier_New:h10:cANSI   " set font  

"syntax on           " grammer highlight

autocmd InsertLeave * se nocul  " highlight current line

autocmd InsertEnter * se cul    " highlight current line

"set ruler           " show scale

set showcmd         " show command 

"set cmdheight=1     " set command height  

"set whichwrap+=<,>,h,l   " backspace

"set scrolloff=3     " 3  

set novisualbell    " no visualbell

set statusline=%F%m%r%h%w\ [FORMAT=%{&ff}]\ [TYPE=%Y]\ [POS=%l,%v][%p%%]\ %{strftime(\"%d/%m/%y\ -\ %H:%M\")}   "status bar

set laststatus=1    " show status bar at start up 1 (always show =2)

set foldenable      " fold enable

set foldmethod=manual   " foldmethod:manual  

"set background=dark "background=dark

set nocompatible  " no compatible with vi 

" show Chinese help

if version >= 603

    set helplang=cn

    set encoding=utf-8

endif

" set color

"colorscheme murphy

"font

"if (has("gui_running")) 

"   set guifont=Bitstream\ Vera\ Sans\ Mono\ 10 

"endif 


 
set fencs=utf-8,ucs-bom,shift-jis,gb18030,gbk,gb2312,cp936

set termencoding=utf-8

set encoding=utf-8

set fileencodings=ucs-bom,utf-8,cp936

set fileencoding=utf-8



"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

"""""new file title""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

"new .c,.h,.sh,.java auto file header 

autocmd BufNewFile *.cpp,*.[ch],*.sh,*.java exec ":call SetTitle()" 

""auto header 

func SetTitle() 

    "if file type is .sh 

    if &filetype == 'sh' 

        call setline(1,"\#########################################################################") 

        call append(line(".")+1, "\# Author: erqing_fang") 

        call append(line(".")+2, "\# mail: erqing_fang@realsil.com.cn") 

        call append(line(".")+3, "\# Created Time: ".strftime("%c")) 

        call append(line(".")+4, "\#########################################################################") 

        call append(line(".")+5, "\#!/bin/bash") 

        call append(line(".")+6, "") 

    else 

        call setline(1, "/*************************************************************************") 

        call append(line("."), "    > File Name: ".expand("%")) 

        call append(line(".")+1, "    > Author: erqing_fang") 

        call append(line(".")+2, "    > Mail: erqing_fang@realsil.com.cn ") 

        call append(line(".")+3, "    > Created Time: ".strftime("%c")) 

        call append(line(".")+4, " ************************************************************************/") 

        call append(line(".")+5, "")

    endif

    if &filetype == 'cpp'

        call append(line(".")+6, "#include<iostream>")

        call append(line(".")+7, "using namespace std;")

        call append(line(".")+8, "")

    endif

    if &filetype == 'c'

        call append(line(".")+6, "#include<stdio.h>")

        call append(line(".")+7, "")

    endif

    "auto located at the end of new file

    autocmd BufNewFile * normal G

endfunc 

""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

"keyboard command

""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""



nmap <leader>w :w!<cr>

nmap <leader>f :find<cr>



"select all ctrl+a

map <C-A> ggVGY

map! <C-A> <Esc>ggVGY

map <F12> gg=G

" copy when selected

vmap <C-c> "+y

"blank line 

nnoremap <F2> :g/^\s*$/d<CR> 

"compare file

nnoremap <C-F2> :vert diffsplit 

"new tag

map <M-F2> :tabnew<CR>  

"show files in current directory

map <F3> :tabnew .<CR>  

"show directory in tree mode

map <C-F3> \be  

"F5 for C,C++

map <F5> :call CompileRunGcc()<CR>

func! CompileRunGcc()

    exec "w"

    if &filetype == 'c'

        exec "!g++ % -o %<"

        exec "! ./%<"

    elseif &filetype == 'cpp'

        exec "!g++ % -o %<"

        exec "! ./%<"

    elseif &filetype == 'java' 

        exec "!javac %" 

        exec "!java %<"

    elseif &filetype == 'sh'

        :!./%

    endif

endfunc

"debug for C,C++

map <F8> :call Rungdb()<CR>

func! Rungdb()

    exec "w"

    exec "!g++ % -g -o %<"

    exec "!gdb ./%<"

endfunc

""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

""practical settings

"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

" auto load

set autoread

" quickfix mode

autocmd FileType c,cpp map <buffer> <leader><space> :w<cr>:make<cr>

"auto complete

set completeopt=preview,menu 

"allow plugins

filetype plugin on

"share paste

set clipboard+=unnamed 

"never backup

set nobackup

"make 

:set makeprg=g++\ -Wall\ \ %

"auto save

set autowrite

set ruler                   " status bar scale

set cursorline              " show current line cursor

set magic                   " set magic

set guioptions-=T           " hide tool

set guioptions-=m           " hide menu

"set statusline=\ %<%F[%1*%M%*%n%R%H]%=\ %y\ %0(%{&fileformat}\ %{&encoding}\ %c:%l/%L%)\

" show status bar info

set foldcolumn=0

set foldmethod=indent 

set foldlevel=3 

set foldenable              " start fold

" no compatible with vi

set nocompatible

" syntax

set syntax=on

" noeb

set noeb

" set confirm

set confirm

" auto indent

set autoindent

set cindent

" Tab witdth

set tabstop=4

" set to 4

set softtabstop=4

set shiftwidth=4

" do not replace tab with space

set noexpandtab

" begin with tab

set smarttab

" show line number

set number

" history record

set history=1000

"no swapfile

set nobackup

set noswapfile

"ignorecase when search

set ignorecase

"搜索逐字符高亮

set hlsearch

set incsearch

"gdefault

set gdefault

"encoding

set enc=utf-8

set fencs=utf-8,ucs-bom,shift-jis,gb18030,gbk,gb2312,cp936

"language

set langmenu=zh_CN.UTF-8

set helplang=cn

" status line

"set statusline=%F%m%r%h%w\ [FORMAT=%{&ff}]\ [TYPE=%Y]\ [POS=%l,%v][%p%%]\ %{strftime(\"%d/%m/%y\ -\ %H:%M\")}

"set statusline=[%F]%y%r%m%*%=[Line:%l/%L,Column:%c][%p%%]

" always show status line

set laststatus=2

" height 

set cmdheight=2

" detect file type

filetype on

" 载入文件类型插件

filetype plugin on

" file type indent 

filetype indent on

" global info

set viminfo+=!

" iskeyword

set iskeyword+=_,$,@,%,#,-

" linespace

set linespace=0

" wildmenu

set wildmenu

" backspace

set backspace=2

" backspace

set whichwrap+=<,>,h,l

" mouse

set mouse=

set selection=exclusive

set selectmode=mouse,key

" report

set report=0

" fillchars

set fillchars=vert:\ ,stl:\ ,stlnc:\

" show match

set showmatch

" matchtime

set matchtime=1

" scrolloff

set scrolloff=3

" smartindent

set smartindent

" filetype txt auto highlight

au BufRead,BufNewFile *  setfiletype txt

"inoremap

:inoremap ( ()<ESC>i

:inoremap ) <c-r>=ClosePair(')')<CR>

:inoremap { {<CR>}<ESC>O

:inoremap } <c-r>=ClosePair('}')<CR>

:inoremap [ []<ESC>i

:inoremap ] <c-r>=ClosePair(']')<CR>

function! ClosePair(char)

    if getline('.')[col('.') - 1] == a:char

        return "\<Right>"

    else

        return a:char

    endif

endfunction

filetype plugin indent on 

"completeopt

set completeopt=longest,menu

"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

" CTags settings

"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

let Tlist_Sort_Type = "name"    " sort by name

let Tlist_Use_Right_Window = 1  " right window  

let Tlist_Compart_Format = 1    " compart format

let Tlist_Exist_OnlyWindow = 1  " only window

let Tlist_File_Fold_Auto_Close = 0  " auto close

let Tlist_Enable_Fold_Column = 0    " fold column

autocmd FileType java set tags+=D:\tools\java\tags  

"autocmd FileType h,cpp,cc,c set tags+=D:\tools\cpp\tags  

"let Tlist_Show_One_File=1            "one file

"set tags

set tags=tags  

"set autochdir 



""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

"other

"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

"default open Taglist 

let Tlist_Auto_Open=1 

"""""""""""""""""""""""""""""" 

" Tag list (ctags) 

"""""""""""""""""""""""""""""""" 

let Tlist_Ctags_Cmd = '/usr/bin/ctags' 

let Tlist_Show_One_File = 1 "show one file

let Tlist_Exit_OnlyWindow = 1 "exit

let Tlist_Use_Right_Window = 1 "taglist

" minibufexpl

let g:miniBufExplMapWindowNavVim = 1

let g:miniBufExplMapWindowNavArrows = 1

let g:miniBufExplMapCTabSwitchBufs = 1
let g:miniBufExplModSelTarget = 1
