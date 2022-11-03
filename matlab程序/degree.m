

global a;
a=1;
sboxcomplete([1,2,3])

function is_sboxcomplete = sboxcomplete(inputsbox)
    if a==1
        is_sboxcomplete = 0;
    else
        is_sboxcomplete = 1;
    end
end
