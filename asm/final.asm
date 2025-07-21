; This is the final version of the human readable asm code that gets injected
; It's written in a Cheat Engine format, because thats how i assembled this code

; Cheat engine macros (not relevant to this project)
label(usinglist1)
label(usinglist2)

label(list1loop)
label(list1loopend)

label(list2loop)
label(list2loopend)

label(end)



; Setting data
23889670000:
  dq 23889670000+18

23889670000+8:
  dq 23889670000+338

23889670000+10:
  db 00

23889670000+11:
  db 01

23889670000+12:
  db 01



; Executable code
23889670658:

  ; Save register states
  push rax
  push rcx
  push rdx
  push rsi

  ; Store base address for data
  mov rax,23889670000
  mov rsi,rax       ; Holds the address of the list pointer (different for each loop)

  ; Check which list should be used
  mov cl,[rax+10]
  test cl,cl
  mov rcx,rax     ; Set up for either loop
  jne usinglist2


usinglist1:

  ; Check if list is full
  add rcx,338     ; list1end
  mov rdx,[rsi]
  cmp rdx,rcx
  je end

  ; Set up for the loop
  mov rcx,rax
  add rcx,18      ; list1start
  sub rdx,8


list1loop:

  ; Check if the loop should be done, if all elements have been checked
  cmp rdx,rcx
  jl loopend

  ; Check if rbx equals the value in the list, if yes leave, if no move to next and repeat
  cmp [rdx],rbx
  je end
  sub rdx,8
  jmp list1loop


usinglist2:

  ; Add the extra 8 bytes to rsi, so it points to the second list pointer
  add rsi,8

  ; Check if list is full
  add rcx,658     ; list2end
  mov rdx,[rsi]
  cmp rdx,rcx
  je end

  ; Set up for the loop
  mov rcx,rax
  add rcx,338     ; list2start
  sub rdx,8

list2loop:

  ; Check if the loop should be done, if all elements have been checked
  cmp rdx,rcx
  jl loopend

  ; Check if rbx equals the value in the list, if yes leave, if no move to next and repeat
  cmp [rdx],rbx
  je end
  sub rdx,8
  jmp list2loop


loopend:

  mov rcx,[rsi]
  mov [rcx],rbx
  add rcx,8
  mov [rsi],rcx

end:

  ; Restore register states
  pop rsi
  pop rdx
  pop rcx
  pop rax

  ; Execute original code
  movsd [rbx+1D0],xmm0
  mov [rbx+1D8],eax

  jmp GameAssembly.dll+40C1834
