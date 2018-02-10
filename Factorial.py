def Fact_while(n):
    x = n
    while(x>0):
            x -= 1
            if(x > 0):
                n = n* x
    return n
def Fact_for(n):
    y = n
    for x in range(n):
        y = y-1
        if(y>0):
            n = n * y
    return n
def fact_rec(n):
    if n ==0:
        return 1
    else:
        return n * fact_rec(n-1)
def main():
    print(Fact_while(10),Fact_for(10), fact_rec(10))
main()