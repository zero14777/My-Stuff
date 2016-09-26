def arrangeCoins(coins):
    for coin_count in coins:
        if coin_count < 1:
            print(0)
            continue
            
        n = 1
        while n*(n+1)/2 <= coin_count:
            n = n + 1 
        print(n - 1)
    
    return