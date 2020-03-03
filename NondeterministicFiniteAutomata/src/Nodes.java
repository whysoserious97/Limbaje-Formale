import java.util.*;

public class Nodes {
    public static HashMap<String,Nodes> allNodes=new HashMap<>();
    public static String[] arrOfterminals;
    ///////////////////////////////////////////////////////////////////
    public static Queue<HashSet<Nodes>> q = new LinkedList<>();
    public static HashSet<HashSet<Nodes>> allStates= new HashSet<>();
    public static List<Cell> rows=new LinkedList<>();
    ///////////////////////////////////////////////////////////////////

    String name;
    HashMap<Nodes,HashSet<String>> nextstates;
    HashMap<String,HashSet<Nodes>> nextNodes;
    HashSet<Nodes> nodeset;
    ///////////////////


    public Nodes(String name){
        this.name=name;
        nextstates=new HashMap<>();
        nextNodes=new HashMap<>();
        nodeset=new HashSet<>();
    }
    public static void assign(String previous,String next,String value){
        Nodes key = allNodes.get(next); // se ia nodul cu stringul ce-i caracterizeaza numele
        key.name=next;
        Nodes key0 = allNodes.get(previous);
        key0.nextstates.computeIfAbsent(key, k -> new HashSet<>());
        key0.nextstates.get(key).add(value); //se adauga o valoare in set
    }
    public static void generateTable1(Nodes init) {
        generatehashMap();


        HashSet<Nodes> currentset = new HashSet<>();
        currentset.add(init);
        q.add(currentset);

       while (!q.isEmpty()){
           HashSet<Nodes> next= q.remove();
           allStates.add(next);
            analize(next);

        }
     //   System.out.println(allStates);



    }

    static void analize(HashSet<Nodes> inputSet){
        Cell cell = new Cell();
        cell.initial=inputSet;

        List<HashSet<Nodes>> list = new LinkedList<>();
        if(!cell.initial.isEmpty())
        rows.add(cell);
        for(String j:arrOfterminals){
            HashSet<Nodes> solution= new HashSet<>();
            for (Nodes i:inputSet){
                if(i.nextNodes.containsKey(j))
                {
                    solution.addAll(i.nextNodes.get(j));
                }
            }
            list.add(solution);
            cell.nodeSet=list;
            boolean isPresent=false;
            for(HashSet<Nodes> iterSet:allStates){
                if(equal(iterSet,solution))
                {
                    isPresent=true;
                }
            }

            if(!isPresent){
                q.add(solution);
            }
        }
    }
   static void generatehashMap(){
        for(Nodes nod:allNodes.values()) {
            for (String temr : arrOfterminals) {
                if (!nod.nextNodes.containsKey(temr)) {
                    HashSet<Nodes> table2Set=new HashSet<>();
                    nod.nextNodes.put(temr,table2Set);
                }
                for (Nodes i:nod.nextstates.keySet()){
                    if(nod.nextstates.get(i).contains(temr)){
                        nod.nextNodes.get(temr).add(i);
                    }
                }
            }
          //  System.out.println(nod.nextNodes);
        }

    }
    public static boolean equal(Set<?> set1, Set<?> set2){

        if(set1 == null || set2 ==null){
            return false;
        }

        if(set1.size()!=set2.size()){
            return false;
        }

        return set1.containsAll(set2);

    }
}
