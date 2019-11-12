package paco;

import java.io.IOException;
import java.util.ArrayList;

import it.unimi.dsi.bits.TransformationStrategies;
import it.unimi.dsi.sux4j.mph.PaCoTrieDistributorMonotoneMinimalPerfectHashFunction;

public class PacoDataStructure {

	PaCoTrieDistributorMonotoneMinimalPerfectHashFunction<Long> pacoFunc; 
	
	public PacoDataStructure(ArrayList<Long> initSet) {
		super();
		// TODO Auto-generated constructor stub
		try {
			this.pacoFunc = new PaCoTrieDistributorMonotoneMinimalPerfectHashFunction<Long>(initSet, TransformationStrategies.fixedLong());
			
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}

	public PaCoTrieDistributorMonotoneMinimalPerfectHashFunction<Long> getPacoFunc() {
		return pacoFunc;
	}
	
	public Long getPredecessorIndex(Long num) {
		
		Long result = pacoFunc.getLong(num);
		//System.out.println(pacoFunc.getLong(num));
		return result;
	}
	
public Long getBitDimension() {
		
		//Long result = lcpFunct.getLong(Integer.toUnsignedLong(num));
		//System.out.println(result);
		Long result = pacoFunc.numBits();
		return result;
	}
}
