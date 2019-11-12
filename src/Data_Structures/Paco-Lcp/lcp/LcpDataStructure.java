package lcp;

import java.io.IOException;
import java.util.ArrayList;

import it.unimi.dsi.bits.TransformationStrategies;
import it.unimi.dsi.sux4j.mph.LcpMonotoneMinimalPerfectHashFunction;

public class LcpDataStructure {
	
	private LcpMonotoneMinimalPerfectHashFunction.Builder<Long> LcpBuilder = new LcpMonotoneMinimalPerfectHashFunction.Builder<Long>();
	private LcpMonotoneMinimalPerfectHashFunction<Long> lcpFunct;
	
	
	
	public LcpDataStructure(ArrayList<Long> initSet, Integer sign) {
		super();
		try {
			//System.out.println(initSet.size());
			this.LcpBuilder.keys(initSet);

			this.LcpBuilder.numKeys(initSet.size());
			this.LcpBuilder.signed(sign);
			//this.LcpBuilder.signed(0);
			this.LcpBuilder.transform(TransformationStrategies.fixedLong());
			this.lcpFunct = LcpBuilder.build();
		
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}
	
	public Long getPredecessorIndex(Long num) {
		
		//Long result = lcpFunct.getLong(Integer.toUnsignedLong(num));
		//System.out.println(result);
		Long result = lcpFunct.getLong(num);
		return result;
	}
	
	public Long getBitDimension() {
		
		//Long result = lcpFunct.getLong(Integer.toUnsignedLong(num));
		//System.out.println(result);
		Long result = lcpFunct.numBits();
		return result;
	}
	
	public LcpMonotoneMinimalPerfectHashFunction getLcpFunct() {
		return lcpFunct;
	}
	
	public LcpMonotoneMinimalPerfectHashFunction.Builder<Long> getLcpBuilder() {
		return LcpBuilder;
	}
}
