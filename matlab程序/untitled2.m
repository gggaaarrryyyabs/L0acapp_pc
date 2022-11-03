% clear
% sbox=[0 1 12  3 8 0 8 14 1 2 8 5 8 2 13 9];
sbox=[0    8   20   57   57   23   29   22   26   57   39   33   62   59   51   19   14   42   30   31   35   33    3   36    5   10   60   22   53   28   60   48   29    1   21   44   23   45   47   48   30   41   63   45    9   24   24   44   31   47   19    6    1   23   61   14 13   22   40   22   14   51   27    3];
global n weight_ge_three counter coefsum ;

n=6;
weight_ge_three = find(sum(int2bit(0:2^n-1,n))>2)-1;

len = length(weight_ge_three);
deplen = ones(1,2^n);
depth = 2;
ddt = zeros(2^n,2^n); 

%% the parament related to calculate the degree of vbf of the sbox 
counter = zeros(1,len);
coefsum = zeros(n,len);
flag = zeros(2^n,1);

for i=1:2^n
    flag(i) = sboxdegree(sbox(1:i),i);
%     ddt= ddtbynewsbox(sbox,i,ddt);
end
sum(flag)

function is_degletwo = sboxdegree(inputsbox,dep)
    global n weight_ge_three coefsum counter;
    index = find(sum(int2bit(dep-1,n)<=int2bit(weight_ge_three,n)) == n);
    counter(index) = counter(index)+1;
    coefsum(:,index) = bitxor(coefsum(:,index),int2bit(inputsbox(dep),n));
%     for i = 1:length(counter)
%         if 2^(sum(int2bit(weight_ge_three(i),n))) ==  counter(i)
%             if coefsum(:,i) ~= zeros(n,1)
%                 is_degletwo = 0;
%                 return
%             end
%         end
%     end
    if find(sum(coefsum(:,find(2.^(sum(int2bit(weight_ge_three,n)))==counter))))
        is_degletwo = 0;
        return
    end
    is_degletwo = 1;
end

function newddt = ddtbynewsbox(inputsbox,dep,inputddt)
    global n;
    x = dep-1;
    a = 1:2^n-1;
    aplusx = bit2int(bitxor(int2bit(a,n),int2bit(x,n)),n);
    index = find(aplusx <= x);
    if index
        b = bit2int(bitxor(int2bit(inputsbox(aplusx(index)+1),n),int2bit(inputsbox(x+1),n)),n);
        ind = sub2ind([2^n 2^n],index+1,b+1);
        inputddt(ind) = inputddt(ind)+2;
    end
    newddt = inputddt;
end
