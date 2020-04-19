import java.util.HashMap;
import java.util.HashSet;

public class InaccesibleElimination {
    String[] Vn;
    String[] Vt;
    HashMap<String, HashSet<String>> productions;
    HashSet<String> accesible=new HashSet<>();
    public boolean hasChanged=true;
    public InaccesibleElimination(String[] Vn, String[] Vt, HashMap<String, HashSet<String>> productions){
        this.Vn=Vn;
        this.Vt=Vt;
        this.productions=productions;
       // System.out.println(Vn[0]);
        add(Vn[0]);
        remove();
        System.out.println("After removing inaccesible:");
        System.out.println(productions);
        Chomsky chomsky = new Chomsky(Vn,Vt,productions);
    }
    public void remove(){
        HashSet<String> toRemove=new HashSet<>();
        for(String key:productions.keySet()){
            if (!accesible.contains(key)){
                toRemove.add(key);
            }
        }
        for (String key:toRemove){
            productions.remove(key);
        }
    }
    public void add(String str){
        for(String element:productions.get(str)){
            for(int i=0;i<element.length();i++){
                if(Character.isUpperCase(element.charAt(i)) && !accesible.contains(Character.toString(element.charAt(i)))){
                    String string=Character.toString(element.charAt(i));
                    accesible.add(string);
                    // now we add next accesible states
                    add(string);
                }
            }
        }
    }

}
