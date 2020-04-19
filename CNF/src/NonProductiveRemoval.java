import java.util.ArrayList;
import java.util.HashMap;
import java.util.HashSet;
import java.util.List;

public class NonProductiveRemoval {
    String[] Vn;
    String[] Vt;
    HashMap<String, HashSet<String>> productions;
    public boolean hasChanged=true;
    List<Character> Productive;

    public NonProductiveRemoval(String[] Vn, String[] Vt, HashMap<String, HashSet<String>> productions){
        this.productions=productions;
        this.Vn=Vn;
        this.Vt=Vt;
        this.Productive=new ArrayList<>();
        remove();
        System.out.println("After removing non-productive:");
        System.out.println(productions);
        InaccesibleElimination inaccesibleElimination= new InaccesibleElimination(Vn,Vt,productions);

    }
    public void remove(){
        while (hasChanged){
            hasChanged=false;
            for(String key :productions.keySet()){
                for(String element:productions.get(key)){
                    boolean allProductive=true;
                    for (int i=0;i<element.length();i++){
                        if (Character.isUpperCase(element.charAt(i)) && !Productive.contains(element.charAt(i))){
                            allProductive=false;
                        }
                    }
                    if(allProductive){
                        Productive.add(key.charAt(0));
                    }
                }
            }
        }
        // Now the productive elements are determinated
        for(String key:productions.keySet()){
            if (!Productive.contains(key.charAt(0))){
                productions.remove(key);
            }
        }

    }
}
