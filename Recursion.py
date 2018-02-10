def Printing(n):
    if n >= 1:
        Printing(n-1)
        return n
def Multiply(x,y):
    r= 0
    if y ==0:
        return y
    elif y ==1:
        return x
    elif y<0:
        return -(x -Multiply(x,x+1))
    else:
        return x+ Multiply(x,y-1)


def Lines(n):
    if(n >=1):
        Lines(n-1)
        print( '*' * n)
def Largest_List(List):
    if len(List) == 1:
        return List[0]
    else:
        Large = Largest_List(List[1:])
        return Large if Large > List[0] else List[0]
def Sum(n):
    if len(n)> 1:
        return n[0] + Sum(n[1:])
    else:
        return n[0]
def sum_of_numbers(n):
    x = n
    if n == 0:
        return 0
    else:
        return x + sum_of_numbers(n-1)
def powers(n,p):
    x = p
    if n ==0:
        return n
    elif p==0:
        return 1
    elif p>0:
        return n * powers(n,p-1)

def main():
    print(Printing(10))
    print(Multiply(10, 5))
    Lines(8)
    print(Largest_List([1,2,3,4,5,6,7,8]))
    print(sum([1, 2, 3, 4, 5, 6, 7, 8]))
    print(sum_of_numbers(10))
    print(powers(10, 5))
main()