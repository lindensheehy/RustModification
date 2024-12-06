

0x0 :       list1pointer        (8 bytes)
0x8 :       list2pointer        (8 bytes)
0x10 :      listflag            (1 byte)
0x18 :      list1               (100 * 8 = 800 bytes)
0x338 :     list2               (100 * 8 = 800 bytes)
0x658 :     executable memory start


main:
        ; store rip
        push rax
        mov rax,rip
        sub rax,659
        
        ; save register states
        push rbx
        push rcx
        push rdx

        ; check which list should be used
        mov bl,[rax+10]
        test bl,bl
        jz usinglist1
        jmp usinglist2


usinglist1:

        ; check semaphore state
        mov bl,1
        lock xchg [rax+11],bl
        jnz end

        ; check if list is full
        mov rbx,rax
        add rbx,338     ; list1end
        mov rcx,[rax]
        cmp rcx,rbx
        je end

        ; set up for the loop
        mov rbx,rax     
        add rbx,18      ; list1start
        sub rcx,8
        jmp list1loop

list1loop:

        ; check if the loop should be done, if all elements have been checked
        cmp rcx,rbx
        jl list1loopend

        ; check if rdi equals the value in the list, if yes leave, if no move to next and repeat
        cmp [rcx],rdi
        je end
        sub rcx,8
        jmp list1loop


list1loopend:

        mov rbx,[rax]
        mov [rbx],rdi
        add rbx,8
        mov [rax],rbx

        jmp end

usinglist2:

        ; check semaphore state
        mov bl,1
        lock xchg [rax+12],bl
        jnz end

        ; check if list is full
        mov rbx,rax
        add rbx,658     ; list2end
        mov rcx,[rax+8]
        cmp rcx,rbx
        je end

        ; set up for the loop
        mov rbx,rax
        add rbx,338     ; list2start
        sub rcx,8
        jmp list1loop

list2loop:

        ; check if the loop should be done, if all elements have been checked
        cmp rcx,rbx
        jl list2loopend

        ; check if rdi equals the value in the list, if yes leave, if no move to next and repeat
        cmp [rcx],rdi
        je end
        sub rcx,8
        jmp list2loop


list2loopend:

        mov rbx,[rax+8]
        mov [rbx],rdi
        add rbx,8
        mov [rax+8],rbx

        jmp end

end:

        ; restore register states
        pop rdx
        pop rcx
        pop rbx
        pop rax

        ; execute original code
        movsd [rdi+1D0],xmm0
        mov [rdi+1D8],eax

        jmp return



notes
list1pointer at [rax]
list2pointer at [rax+8]
listflag at [rax+10]
list1 at [rax+18]
list2 at [rax+338]



; cheat egnine version
label(usinglist1)
label(usinglist2)

label(list1loop)
label(list1loopend)

label(list2loop)
label(list2loopend)

label(end)



// setting data
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



// executable code
23889670658:

  // save register states
  push rax
  push rcx
  push rdx
  push rsi

  // store base address for data
  mov rax,23889670000
  mov rsi,rax       // holds the address of the list pointer (different for each loop)

  // check which list should be used
  mov cl,[rax+10]
  test cl,cl
  mov rcx,rax     // set up for either loop
  jne usinglist2


usinglist1:

  // check if list is full
  add rcx,338     // list1end
  mov rdx,[rsi]
  cmp rdx,rcx
  je end

  // set up for the loop
  mov rcx,rax
  add rcx,18      // list1start
  sub rdx,8


list1loop:

  // check if the loop should be done, if all elements have been checked
  cmp rdx,rcx
  jl loopend

  // check if rbx equals the value in the list, if yes leave, if no move to next and repeat
  cmp [rdx],rbx
  je end
  sub rdx,8
  jmp list1loop


usinglist2:

  // add the extra 8 bytes to rsi, so it points to the second list pointer
  add rsi,8

  // check if list is full
  add rcx,658     // list2end
  mov rdx,[rsi]
  cmp rdx,rcx
  je end

  // set up for the loop
  mov rcx,rax
  add rcx,338     // list2start
  sub rdx,8

list2loop:

  // check if the loop should be done, if all elements have been checked
  cmp rdx,rcx
  jl loopend

  // check if rbx equals the value in the list, if yes leave, if no move to next and repeat
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

  // restore register states
  pop rsi
  pop rdx
  pop rcx
  pop rax

  // execute original code
  movsd [rbx+1D0],xmm0
  mov [rbx+1D8],eax

  jmp GameAssembly.dll+40C1834


RAW ASSEMBLY:

29907660658 - 50                    - push rax
29907660659 - 51                    - push rcx
2990766065A - 52                    - push rdx
2990766065B - 56                    - push rsi
2990766065C - 48 B8 0000660799020000 - mov rax,newmem
29907660666 - 48 8B F0              - mov rsi,rax
29907660669 - 8A 48 10              - mov cl,[rax+10]
2990766066C - 84 C9                 - test cl,cl
2990766066E - 48 8B C8              - mov rcx,rax
29907660671 - 0F85 36000000         - jne 299076606AD
29907660677 - 48 81 C1 38030000     - add rcx,00000338
2990766067E - 48 8B 16              - mov rdx,[rsi]
29907660681 - 48 39 CA              - cmp rdx,rcx
29907660684 - 0F84 6D000000         - je 299076606F7
2990766068A - 48 8B C8              - mov rcx,rax
2990766068D - 48 83 C1 18           - add rcx,18
29907660691 - 48 83 EA 08           - sub rdx,08
29907660695 - 48 39 CA              - cmp rdx,rcx
29907660698 - 0F8C 4C000000         - jl 299076606EA
2990766069E - 48 39 1A              - cmp [rdx],rbx
299076606A1 - 0F84 50000000         - je 299076606F7
299076606A7 - 48 83 EA 08           - sub rdx,08
299076606AB - EB E8                 - jmp 29907660695
299076606AD - 48 83 C6 08           - add rsi,08
299076606B1 - 48 81 C1 58060000     - add rcx,00000658
299076606B8 - 48 8B 16              - mov rdx,[rsi]
299076606BB - 48 39 CA              - cmp rdx,rcx
299076606BE - 0F84 33000000         - je 299076606F7
299076606C4 - 48 8B C8              - mov rcx,rax
299076606C7 - 48 81 C1 38030000     - add rcx,00000338
299076606CE - 48 83 EA 08           - sub rdx,08
299076606D2 - 48 39 CA              - cmp rdx,rcx
299076606D5 - 0F8C 0F000000         - jl 299076606EA
299076606DB - 48 39 1A              - cmp [rdx],rbx
299076606DE - 0F84 13000000         - je 299076606F7
299076606E4 - 48 83 EA 08           - sub rdx,08
299076606E8 - EB E8                 - jmp 299076606D2
299076606EA - 48 8B 0E              - mov rcx,[rsi]
299076606ED - 48 89 19              - mov [rcx],rbx
299076606F0 - 48 83 C1 08           - add rcx,08
299076606F4 - 48 89 0E              - mov [rsi],rcx
299076606F7 - 5E                    - pop rsi
299076606F8 - 5A                    - pop rdx
299076606F9 - 59                    - pop rcx
299076606FA - 58                    - pop rax
299076606FB - F2 0F11 83 D0010000   - movsd [rbx+000001D0],xmm0
29907660703 - 89 83 D8010000        - mov [rbx+000001D8],eax
29907660709 - FF25 00000000 34189C7CF97F0000 - jmp GameAssembly.dll+40C1834


