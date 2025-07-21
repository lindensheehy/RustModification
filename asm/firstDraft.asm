; This is my initial draft of the internal asm code for this project
; While this is largely technically complete, it does not mirror any standard asm style
; This is just a sketch I wrote to map out the logic before porting to Cheat Engine


; Some general information:
;
;   0x0 :       list1pointer        (8 bytes)
;   0x8 :       list2pointer        (8 bytes)
;   0x10 :      listflag            (1 byte)
;   0x18 :      list1               (100 * 8 = 800 bytes)
;   0x338 :     list2               (100 * 8 = 800 bytes)
;   0x658 :     executable memory start
;
;   list1pointer    -> [rax]
;   list2pointer    -> [rax+8]
;   listflag        -> [rax+10]
;   list1           -> [rax+18]
;   list2           -> [rax+338]


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
