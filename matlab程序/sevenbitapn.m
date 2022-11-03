%% New_Instances_of_Quadratic_APN_Functions
%% APN_check;Algebraic_check;

%% Input: the size of whole sbox is 2^n with initialized to sbox(i)=-1 (undefined) for all 0<=i<=2^n-1, 
%% Input: the permutation matrix with size 2^n*2^n with all P(i) belonging to [0,1,...,2^n-1].
%% Output: an n-bit APN functions F of degree no more than 2.
clear

global n weight_ge_three counter coefsum weightof_wgt ddt;
n = 7;


weight_ge_three = find(sum(int2bit(0:2^n-1,n))>2)-1;
weightof_wgt = 2.^(sum(int2bit(weight_ge_three,n)));
len_weight = length(weight_ge_three);

%% generate 2^n permutation for 0..2^n-1 without pairwise different
original = zeros(2^n,2^n);

for i=1:2^n
    original(i,:)=randperm(2^n,2^n)-1;
end
%% initialize sbox and depth flag
ddt = zeros(2^n,2^n); 
sbox = -ones(1,2^n);
sbox(1) = 0;
deplen = ones(1,2^n);
depth = 2;


%% the parament related to calculate the degree of vbf of the sbox 
coefsum = zeros(n,len_weight);
counter = zeros(1,len_weight);

%%
apnflag = sboxdegree(sbox,1);

iteration = 0;
while sbox(2^n) == -1 % running codes until the sbox has no undefined element(-1).
    iteration = iteration+1
    %%
    len = deplen(depth); 
    sbox(depth) = original(depth,len); % assign the value for the sbox[depth]
    apnflag = ddtbynewsbox(sbox,depth); % get new ddt of the sbox with values [sbox[0],sbox[1],...,sbox[depth]]
    if apnflag
        degreeflag = sboxdegree(sbox,depth); % test whether the degree of vbf of the sbox is less than 2
        if degreeflag
            depth = depth+1; % assign the value for the sbox[depth+1]
            continue
        else
            removesboxdegree(sbox,depth);
            ddtbyfailsbox(sbox,depth);
        end
    end
    sbox(depth) = -1;
    deplen(depth) = deplen(depth)+1;
    while max(deplen) == 2^n+1
        deplen(depth) = 1;
        depth = depth-1;
        removesboxdegree(sbox,depth);
        ddtbyfailsbox(sbox,depth);
        sbox(depth) = sbox(depth)-1;
        deplen(depth) = deplen(depth)+1;
    end
end

sbox

function is_degletwo = sboxdegree(inputsbox,dep)
    global n weight_ge_three coefsum counter weightof_wgt;
    index = find(sum(int2bit(dep-1,n) <= int2bit(weight_ge_three,n)) == n);
    counter(index) = counter(index)+1;
    coefsum(:,index) = bitxor(coefsum(:,index),int2bit(inputsbox(dep),n));
    if find(sum(coefsum(:,find(weightof_wgt == counter))))
        is_degletwo = 0;
        return
    end
    is_degletwo = 1;
end

function removesboxdegree(inputsbox,dep)
    global n weight_ge_three coefsum counter;
    index = find(sum(int2bit(dep-1,n) <= int2bit(weight_ge_three,n)) == n);
    counter(index) = counter(index)-1;
    coefsum(:,index) = bitxor(coefsum(:,index),int2bit(inputsbox(dep),n));
end
 
function is_apn = ddtbynewsbox(inputsbox,dep)
    global n ddt;
    a = 1:2^n-1;
    aplusx = bitxor(a,dep-1);
    index = find(aplusx <= dep-1);
    if index
        b = bitxor(inputsbox(aplusx(index)+1),inputsbox(dep));
        ind = sub2ind([2^n 2^n],index+1,b+1);
        if find(ddt(ind) == 2)
            is_apn = 0;
            return
        end        
        ddt(ind) = ddt(ind)+2;
    end
    is_apn = 1;
end

function ddtbyfailsbox(inputsbox,dep)
    global n ddt;
    a = 1:2^n-1;
    aplusx = bitxor(a,dep-1);
    index = find(aplusx <= dep-1);
    if index
        b = bitxor(inputsbox(aplusx(index)+1),inputsbox(dep));
        ind = sub2ind([2^n 2^n],index+1,b+1);
        ddt(ind) = ddt(ind)-2;
    end
end


  
    

        