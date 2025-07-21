; This file contains the same assembly as final.asm, but in the form of raw bytes
; This code is injected in the format of raw bytes. These are those bytes

; Save register states
658 - 50                        - push rax
659 - 51                        - push rcx
65A - 52                        - push rdx
65B - 56                        - push rsi

; Cheat logic
65C - 48 B8 0000660799020000    - mov rax,newmem
666 - 48 8B F0                  - mov rsi,rax
669 - 8A 48 10                  - mov cl,[rax+10]
66C - 84 C9                     - test cl,cl
66E - 48 8B C8                  - mov rcx,rax
671 - 0F85 36000000             - jne 299076606AD
677 - 48 81 C1 38030000         - add rcx,00000338
67E - 48 8B 16                  - mov rdx,[rsi]
681 - 48 39 CA                  - cmp rdx,rcx
684 - 0F84 6D000000             - je 299076606F7
68A - 48 8B C8                  - mov rcx,rax
68D - 48 83 C1 18               - add rcx,18
691 - 48 83 EA 08               - sub rdx,08
695 - 48 39 CA                  - cmp rdx,rcx
698 - 0F8C 4C000000             - jl 299076606EA
69E - 48 39 1A                  - cmp [rdx],rbx
6A1 - 0F84 50000000             - je 299076606F7
6A7 - 48 83 EA 08               - sub rdx,08
6AB - EB E8                     - jmp 29907660695
6AD - 48 83 C6 08               - add rsi,08
6B1 - 48 81 C1 58060000         - add rcx,00000658
6B8 - 48 8B 16                  - mov rdx,[rsi]
6BB - 48 39 CA                  - cmp rdx,rcx
6BE - 0F84 33000000             - je 299076606F7
6C4 - 48 8B C8                  - mov rcx,rax
6C7 - 48 81 C1 38030000         - add rcx,00000338
6CE - 48 83 EA 08               - sub rdx,08
6D2 - 48 39 CA                  - cmp rdx,rcx
6D5 - 0F8C 0F000000             - jl 299076606EA
6DB - 48 39 1A                  - cmp [rdx],rbx
6DE - 0F84 13000000             - je 299076606F7
6E4 - 48 83 EA 08               - sub rdx,08
6E8 - EB E8                     - jmp 299076606D2
6EA - 48 8B 0E                  - mov rcx,[rsi]
6ED - 48 89 19                  - mov [rcx],rbx
6F0 - 48 83 C1 08               - add rcx,08
6F4 - 48 89 0E                  - mov [rsi],rcx

; Restore register states
6F7 - 5E                        - pop rsi
6F8 - 5A                        - pop rdx
6F9 - 59                        - pop rcx
6FA - 58                        - pop rax

; Original instructions (those that were patched over by the hook)
6FB - F2 0F11 83 D0010000       - movsd [rbx+000001D0],xmm0
703 - 89 83 D8010000            - mov [rbx+000001D8],eax

; Jump back to hook site
709 - FF25 00000000 34189C7CF97F0000 - jmp GameAssembly.dll+40C1834
