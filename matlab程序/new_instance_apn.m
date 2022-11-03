%% New_Instances_of_Quadratic_APN_Functions
%% APN_check;Algebraic_check;

%% Input: the size of whole sbox is 2^n with initialized to sbox(i)=0 (undefined) for all 0<=i<=2^n-1, 
%% Input: the permutation matrix with size 2^n*2^n with all P(i) belonging to [0,1,...,2^n-1].
%% Output: an n-bit APN functions F of degree no more than 2.
clear

global n weight_ge_three ;
n = 7;

%% generate a permutation for P(i)
original = [];
for i=1:2^n
    original(i,:)=randperm(2^n)-1;
end

sbox = -ones(1,2^n);
sbox(1) = 0;
deplen = ones(1,2^n);
depth = find(sbox==-1,1);

weight_ge_three = find(sum(int2bit(0:2^n-1,n,'left-msb'))>2)-1;
counter = zeros(1,length(weight_ge_three));
coefsum = zeros(length(weight_ge_three),n);

ddt = zeros(2^n,2^n); %% construct DDT with all values for only one colomnum;

while sboxcomplete(sbox) == 0
    len = deplen(depth); 
    sbox(depth) = original(depth,len);
    ddt = ddtbynewsbox(sbox,depth,ddt);
    apnflag = apncheck(ddt);
    degreeflag = sboxdegree(sbox,depth,counter,coefsum);
    
    if apnflag & degreeflag 
        depth = depth+1;
        sbox
    else
        removesboxdegree(sbox,depth,counter,coefsum);
        ddt = ddtbyfailsbox(sbox,depth,ddt);
        sbox(depth) = -1;
        deplen(depth) = deplen(depth)+1;
        while max(deplen)>2^n 
            deplen(depth) = 1;
            depth = depth-1;
            removesboxdegree(sbox,depth,counter,coefsum);
            ddt = ddtbyfailsbox(sbox,depth,ddt);
            sbox(depth) = sbox(depth)-1;
            deplen(depth) = deplen(depth)+1;
        end 
    end
end

sbox

function is_sboxcomplete = sboxcomplete(inputsbox)
    global n;
    if inputsbox(2^n) == -1
        is_sboxcomplete = 0;
    else
        is_sboxcomplete = 1;
    end
end

function is_apncheck = apncheck(inputddt)
    if max(inputddt)>2
        is_apncheck = 0;
    else
        is_apncheck = 1;
    end
end

function is_degletwo = sboxdegree(inputsbox,dep,coef_counter,coef_sum)
    global n weight_ge_three;
    u = weight_ge_three(i);
    index = find(sum(de2bi(dep-1,n,'left-msb')<=de2bi(weight_ge_three,n,'left-msb'),2) == n);
    coef_counter(index) = coef_counter(index)+1;
    coef_sum(index,:) = bitxor(coef_sum(index),de2bi(inputsbox(dep),n,'left-msb'));
    coef_counter(index) = coef_counter(index)+1;
    coef_sum(index,:) = bitxor(coef_sum(index),de2bi(inputsbox(dep),n,'left-msb'));
    if coef_counter(i) == 2^(sum(de2bi(u,n)))
        if coef_sum(i) ~= zeros(1,n)
            is_degletwo = 0;
        end
    end
    end
    is_degletwo = 1;
end

function removesboxdegree(inputsbox,dep,coef_counter,coef_sum)
    global n weight_ge_three;
    for i = 1:length(weight_ge_three)
        u = weight_ge_three(i);
        if sum(de2bi(dep-1,n,'left-msb')<=de2bi(u,n,'left-msb')) == n
            coef_counter(i) = coef_counter(i)-1;
            coef_sum(i,:) = bitxor(coef_sum(i),de2bi(inputsbox(dep),n,'left-msb'));
        end
    end
end
 
function newddt = ddtbynewsbox(inputsbox,dep,inputddt)
    global n;
    x = dep-1;
    for a = 1:2^n-1
        aplusx = bit2int(bitxor(de2bi(a,n,'left-msb'),de2bi(x,n,'left-msb'))',n);
        if aplusx <= dep-1
            b = bit2int(bitxor(de2bi(inputsbox(aplusx+1),n,'left-msb'),de2bi(inputsbox(x+1),n,'left-msb'))',n);
            inputddt(a+1,b+1) = inputddt(a+1,b+1)+2;
        end
    end
    newddt = inputddt;
end

function oldddt = ddtbyfailsbox(inputsbox,dep,inputddt)
    global n;
    x = dep-1;
    for a = 1:2^n-1
        aplusx = bit2int(bitxor(de2bi(a,n,'left-msb'),de2bi(x,n,'left-msb'))',n);
        if aplusx <= dep-1
            b = bit2int(bitxor(de2bi(inputsbox(aplusx+1),n,'left-msb'),de2bi(inputsbox(x+1),n,'left-msb'))',n);
            inputddt(a+1,b+1) = inputddt(a+1,b+1)-2;
        end
    end
    oldddt = inputddt;
end


  
    

        