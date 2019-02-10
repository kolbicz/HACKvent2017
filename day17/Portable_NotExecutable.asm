.686P
MODEL FLAT, STDCALL
JUMPS
LOCALS

UNICODE=0

INCLUDE C:\TASM\W32.INC
INCLUDE MD5.ASM

EXTRN 	RtlMoveMemory		: PROC

.DATA?

hApp	dd ?
hIcon	dd ?

szFlag	db 30 dup (?)

.DATA

szClassName		db "HACKvent_Class",0
szAppName		db "HACKvent 2017 - Portable NotExecutable",0

stWinClass 	WNDCLASSEX 	<>
stMessage	MSG			<>

.CODE

Start: 

	call 	GetModuleHandleA, NULL
	mov		hApp, eax
	call	WinMain, hApp, NULL, NULL, SW_SHOWDEFAULT
	call	ExitProcess, NULL

WinMain	PROC, hDlg:DWORD, hPrevInst:DWORD, CmdLine:LPSTR, CmdShow:DWORD 

	mov		stWinClass.wc_cbSize, size WNDCLASSEX
	mov		stWinClass.wc_style, NULL
	mov		stWinClass.wc_lpfnWndProc, offset WndProc
	mov		stWinClass.wc_cbClsExtra, NULL
	mov		stWinClass.wc_cbWndExtra, NULL
	push	hApp
	pop		stWinClass.wc_hInstance
	call	LoadIcon, hApp, 5000
	mov		hIcon, eax
	mov		stWinClass.wc_hIcon, eax
	call	LoadCursor, NULL, IDC_ARROW
	mov		stWinClass.wc_hCursor, eax
	mov		stWinClass.wc_hbrBackground, COLOR_WINDOW
	mov		stWinClass.wc_lpszMenuName, NULL
	mov		stWinClass.wc_lpszClassName, offset szClassName
	call	RegisterClassEx, offset stWinClass
	call	CreateWindowEx, WS_EX_CLIENTEDGE, offset szClassName, offset szAppName, \
			WS_OVERLAPPEDWINDOW-WS_MAXIMIZEBOX-WS_THICKFRAME, CW_USEDEFAULT, CW_USEDEFAULT, \
			400, 120, NULL, NULL, hApp, NULL
	mov		hDlg, eax
	call	ShowWindow, hDlg, SW_NORMAL
    call 	UpdateWindow, hDlg
    .WHILE TRUE
        call	GetMessage, offset stMessage, NULL, 0, 0 
        .BREAK .IF eax==FALSE
             call	IsDialogMessage, hDlg, offset stMessage
        	.IF	eax==FALSE
				call 	TranslateMessage, offset stMessage
				call	DispatchMessage, offset stMessage
			.ENDIF
    .ENDW
    mov     eax, stMessage.ms_wParam 
    ret
    
WinMain	ENDP

.DATA

; user interface definitions

szEditClass	db "EDIT",0
szButClass	db "BUTTON",0
szOk		db "Flag",0
szExit		db "Exit",0
szFontName	db "Tahoma",0
szBase		db "7pQmc1Hnw4j0fEubLT3etr8BJVN2KDGSolO5IhX9WsyYdAFRzPkxCqg6ZUivaM",0

.DATA?

; global handles

hLib		dd ?
hEditName	dd ?
hEditSerial	dd ?
hEdit		dd ?
hFont		dd ?
hFile		dd ?
hMapping	dd ?
pFileMap	dd ?

szFileName db MAX_PATH dup (?)
md5 db 16 dup (?)
md6 db 16 dup (?)


.CODE

WndProc	PROC, hDlg:DWORD, uMsg:UINT, wParam:DWORD, lParam:DWORD
	
	.IF		uMsg==WM_CREATE
		call	CreateWindowEx, WS_EX_CLIENTEDGE, offset szEditClass, NULL,\ 
        		WS_CHILD+WS_VISIBLE+ES_LEFT+ES_AUTOHSCROLL+WS_TABSTOP+ES_CENTER,\ 
        		12, 20, 366, 20, hDlg, 2000, hApp, NULL
		mov		hEditSerial, eax
		call	CreateWindowEx, WS_EX_STATICEDGE, offset szButClass, offset szOk,\ 
                WS_CHILD+WS_VISIBLE+WS_TABSTOP,\ 
                12, 50, 60, 28, hDlg, 3000, hApp, NULL
		call	SetFocus, eax		                
		call	CreateWindowEx, WS_EX_STATICEDGE, offset szButClass, offset szExit,\ 
                WS_CHILD+WS_VISIBLE+WS_TABSTOP,\ 
                318, 50, 60, 28, hDlg, 4000, hApp, NULL                
		call	CenterWindow, hDlg
       	call	CreateFontA, 16, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET, \
        		OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, offset szFontName
       	mov		hFont, eax			
       	call	SendDlgItemMessage, hDlg, 2000, WM_SETFONT, HFONT, NULL
       	call	SendDlgItemMessage, hDlg, 3000, WM_SETFONT, HFONT, NULL		
       	call	SendDlgItemMessage, hDlg, 4000, WM_SETFONT, HFONT, NULL     
	.ELSEIF	uMsg==WM_COMMAND
		and 	wParam, 0FFFFh
		.IF		wParam==3000
			call	GetModuleFileNameA, NULL, offset szFileName, MAX_PATH
			call	CreateFile, offset szFileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL 
			mov		hFile, eax
			call	CreateFileMapping, hFile, NULL, PAGE_READONLY,0,0,0
			mov		hMapping, eax
			call	MapViewOfFile,hMapping,FILE_MAP_READ,0,0,0
			mov		pFileMap, eax
			call	MD5_Compute, pFileMap, 300h, offset md5
			mov		eax, pFileMap
			add		eax, 0300h
			call	MD5_Compute, eax, 300h, offset md6
    		call	CloseHandle, hMapping
    		call	CloseHandle, hFile
    		xor	ecx, ecx
    		.WHILE !ecx==29
    			movzx eax, byte ptr [md5+ecx]
    			mov seed, eax
    			call random, 61
    			inc eax
    			movzx edx, byte ptr [szBase+eax]
    			mov byte ptr [szFlag+ecx], dl
    			inc ecx
    		.ENDW
    		mov	dword ptr [szFlag], "71VH"
    		mov byte ptr [szFlag+4], "-"
    		mov byte ptr [szFlag+9], "-"
    		mov byte ptr [szFlag+14], "-"
    		mov byte ptr [szFlag+19], "-"
    		mov byte ptr [szFlag+24], "-"
			call	SendDlgItemMessage, hDlg, 2000, WM_SETTEXT, NULL, offset szFlag
		.ELSEIF	wParam==4000
			call	PostQuitMessage, NULL
		.ENDIF
  	.ELSEIF	uMsg==WM_DESTROY
    	call	PostQuitMessage, NULL
	.ELSE
        call	DefWindowProc, hDlg, uMsg, wParam, lParam
   	.ENDIF
   	ret

WndProc	ENDP

.DATA?

seed dd ?

.CODE

random PROC base:DWORD         ; Park Miller random number algorithm
USES ecx
    mov eax, seed              ; from M32lib/nrand.asm
    xor edx, edx
    mov ecx, 127773
    div ecx
    mov ecx, eax
    mov eax, 16807
    mul edx
    mov edx, ecx
    mov ecx, eax
    mov eax, 2836
    mul edx
    sub ecx, eax
    xor edx, edx
    mov eax, ecx
    mov seed, ecx
    div base
    mov eax, edx
    ret
random ENDP

CenterWindow	PROC, hDlg:DWORD

.DATA

stDlgRect	RECT	<>
stDeskRect	RECT	<>

.CODE

LOCAL	DlgHight:DWORD
LOCAL	DlgWidth:DWORD
LOCAL	Dlg_X:DWORD
LOCAL	Dlg_Y:DWORD

	call	GetWindowRect, hDlg, offset stDlgRect
	call	GetDesktopWindow
	call	GetWindowRect, eax, offset stDeskRect
	mov		eax, stDlgRect.rc_bottom
	sub		eax, stDlgRect.rc_top
	mov		DlgHight, eax
	mov		eax, stDlgRect.rc_right
	sub		eax, stDlgRect.rc_left
	mov		DlgWidth, eax
	mov		eax, stDeskRect.rc_bottom
	sub		eax, DlgHight
	shr		eax, 1
	mov		Dlg_Y, eax
	mov		eax, stDeskRect.rc_right
	sub		eax, DlgWidth
	shr		eax, 1
	mov		Dlg_X, eax
	call	MoveWindow, hDlg, Dlg_X, Dlg_Y, DlgWidth, DlgHight, TRUE
	xor		eax, eax
	ret
	
CenterWindow	ENDP

End Start	
