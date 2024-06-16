def ttc(n, out_agents, prefs):
    visited = [0] * n
    current_favorite = [0] * n
    unfinalized = set(range(n))
    iteration = 1
    
    while len(unfinalized) > 0:

        t = next(iter(unfinalized))
        
        # Finde Zyklus
        while visited[t] != iteration:
            visited[t] = iteration

            # Falls notwendig, ziehe neue Kante zum nächst favorisierten, existenten Gut
            while prefs[t][current_favorite[t]] not in unfinalized:
                current_favorite[t] += 1
                
            out_agents[t] = prefs[t][current_favorite[t]]
            
            t = out_agents[t]

        # Laufe Zyklus ab und finalisiere die beteiligten Agenten 
        c_start = t
        unfinalized.remove(c_start)
        c_elem = out_agents[c_start]

        while c_elem != c_start:
            unfinalized.remove(c_elem)
            c_elem = out_agents[c_elem]
        
        iteration += 1

    return out_agents

if __name__ == "__main__":
    n = 5
    
    prefs = [
             [3, 0, 2, 1, 4], 
             [1, 3, 2, 0, 4], 
             [2, 4, 0, 1, 3], 
             [4, 0, 3, 2, 1], 
             [4, 2, 1, 3, 0]
            ]
    
    # Stelle ausgehende Kanten der Agenten dar
    out_agents = [prefs[i][0] for i in range(n)]

    stable_alloc = ttc(n, out_agents, prefs)
    
    print(stable_alloc)