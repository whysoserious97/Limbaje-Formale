import javafx.beans.binding.StringBinding;

import java.util.*;

public class Chomsky {
    List<String> Vn=new ArrayList<>();
    String[] Vt;
    HashMap<String, HashSet<String>> productions;
    HashMap<String, HashSet<String>> toRemove=new HashMap<>();
    HashMap<String, HashSet<String>> toAdd=new HashMap<>();
    HashMap<String,String> shortermap=new HashMap<>();
    HashMap<String,String> mask=new HashMap<>();
    boolean hasChanged=true;
    int j=65;// charCode for geting non-used nonTerminals
    public Chomsky(String[] Vn, String[] Vt, HashMap<String, HashSet<String>> productions){
        this.productions=productions;
        this.Vn.addAll(Arrays.asList(Vn));
        this.Vt=Vt;
        replaceWithNonTerminal();
        System.out.println("Now rules need to derive in maximum 2 Non-terminal and need to add new production in rules:");
        makeShorter();
        System.out.println(productions);
      //  System.out.println(productions);
    }
    public void makeShorter(){
        while (hasChanged){
            hasChanged=false;

            toRemove=new HashMap<>();
            toAdd=new HashMap<>();

            for (String key:productions.keySet()){
                for(String element:productions.get(key)){
                    if(element.length()>2 ){
                        HashSet<String> set = new HashSet<>();
                        HashSet<String> removeset= toRemove.get(key);
                        if(removeset==null){
                            removeset=new HashSet<>();
                            toRemove.put(key,removeset);
                        }
                        if(!shortermap.containsKey(element.substring(0,2))){
                        removeset.add(element);
                        shortermap.put(element.substring(0,2),Character.toString(++j));
                        set.add(element.replace(element.substring(0,2),Character.toString(j)));}
                        else {
                            removeset.add(element);
                            String str=element.replace(element.substring(0,2),shortermap.get(element.substring(0,2)));
                            set.add(str);
                        }
                        toAdd.put(key,set);
                    }
                }
            }
            for (String key:toRemove.keySet()){
                HashSet<String> setToAdd= toAdd.get(key);
                HashSet<String> setRemove=toRemove.get(key);
                for(String element:setRemove){
                    productions.get(key).remove(element);
                }
                for(String element:setToAdd){
                    productions.get(key).add(element);
                }
                for (String key2:shortermap.keySet()){
                    HashSet<String> set = new HashSet<>();
                    set.add(key2);
                    productions.put(shortermap.get(key2),set);
                }

            }
        }
    }
    public void replaceWithNonTerminal(){

        for(String key:productions.keySet()){
            if(j<key.charAt(0)){
                j=key.charAt(0);
            }
        }
        for(String terminal:Vt){
            mask.put(Character.toString(++j),terminal);
        }
        for(String key:productions.keySet()){
            for(String element:productions.get(key)){
                String str = element;//needed to avoid an issue when element need to be replaced with 2 elements ex: 'ab'
                for(String keymask:mask.keySet()){
                    if(str.contains(mask.get(keymask))){
                        if(str.length()>1)
                        str=str.replace(mask.get(keymask),keymask);
                        //now we need to add old value in the set from 'toRemove' map

                    }
                }
                HashSet<String> set=toRemove.get(key);
                if(set==null){
                    set=new HashSet<>();
                    toRemove.put(key,set);
                }
                set.add(element);
                //now we need to add new value in the set from 'toAdd' map
                set=toAdd.get(key);
                if(set==null){
                    set=new HashSet<>();
                    toAdd.put(key,set);
                }
                set.add(str);
                System.out.println("In "+key+" '"+element+"' was replaced with: "+str);
            }
        }
        // Now we want to remove old values and add new values
        for(String key:toRemove.keySet()){
            for(String element:toRemove.get(key)){
                productions.get(key).remove(element);
            }
        }
        //now we want to add the element
        for(String key:toAdd.keySet()){
            for(String element:toAdd.get(key)){
                productions.get(key).add(element);
            }
        }

        // also we need to have productions from mask in rules.
        for(String key:mask.keySet()){
            HashSet<String> set = new HashSet<>();
            set.add(mask.get(key));
            productions.put(key,set);
        }
        System.out.println(productions);

    }

   /* public void replaceWithNonTerminal(){
     for(String key:productions.keySet()){
         for(String element:productions.get(key)){
             for(int i = 0;i<element.length();i++){

                 if(Character.isLowerCase(element.charAt(i))){
                     boolean found=false;
                     for(String key2:productions.keySet()){
                         for(String element2:productions.get(key2)){
                                if(element2.equals(Character.toString(element.charAt(i))) && productions.get(key2).size()==1){
                                    element=element.replace(element.charAt(i),key2.charAt(0));
                                    found=true;
                                }
                         }
                     }
                     if(!found){
                         int j=65;
                         while (productions.keySet().contains(Character.toString(j))){
                            j++;
                         }
                         HashSet<String> temp=new HashSet<>();
                         temp.add(Character.toString(element.charAt(i)));
                         productions.put(Character.toString(j),temp);
                         ////////////////////////////////////////////////////Here I have been stoped
                     }
                 }
             }
         }
     }
    }*/

}
