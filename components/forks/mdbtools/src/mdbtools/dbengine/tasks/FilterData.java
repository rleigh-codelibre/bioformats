package mdbtools.dbengine.tasks;

import mdbtools.dbengine.Data;
import mdbtools.dbengine.MemoryData;
import mdbtools.dbengine.functions.Function;
import mdbtools.dbengine.sql.Condition;
import mdbtools.dbengine.sql.Equation;
import mdbtools.dbengine.sql.FQColumn;
import mdbtools.dbengine.sql.FunctionDef;

import java.sql.SQLException;

/**
 * filter a given result set by a given constraint
 */
public class FilterData implements Task
{
  private MemoryData result;
  private Object where;
  private Task task;
  private int[] tableMap;

  public FilterData(Task task,Object where,int[] tableMap)
  {
    this.task = task;
    this.where = where;
    this.tableMap = tableMap;
  }

  public void run()
    throws SQLException
  {
    MemoryData data = (MemoryData)task.getResult();
    result = new MemoryData();
    for (int i = 0; data.next(); i++)
    {
      if (whereCheckRow(data,where))
        result.addRow(data.getRow(i));
    }
  }

  public Object getResult()
  {
    return result;
  }

  /**
   * Check the where constraint against a single row
   * @param data
   * @param where
   * @return
   */
  private boolean whereCheckRow(Data data,Object where)
    throws SQLException
  {
    if (where instanceof Equation)
      return whereCheckRowEquation(data,(Equation)where);
    if (where instanceof Condition)
      return whereCheckRowConditon(data,(Condition)where);
    else
      throw new RuntimeException("uknown where type: " + where.getClass().getName());
  }

  /**
   * Check an equation portion of the where contraints against a single row
   * @param data
   * @param eq
   * @return
   */
  private boolean whereCheckRowEquation(Data data,Equation eq)
    throws SQLException
  {
    Object left = resolveColumn(data,eq.getLeft());
    Object right = resolveColumn(data,eq.getRight());
    switch(eq.getOperator())
    {
      case Equation.EQUALS:
        return left.equals(right);
      case Equation.NOT_EQUALS:
        return left.equals(right) == false;
      case Equation.LESS_THAN:
        return ((Comparable)left).compareTo((Comparable)right) < 0;
      case Equation.GREATER_THAN:
        return ((Comparable)left).compareTo((Comparable)right) > 0;
      default:
        throw new RuntimeException("unknown equation operator: " + eq.getOperator());
    }
  }

  /**
   * Check a condtion portion of the where constraint against a single row
   * left and right of conditon is either another condition or an equation
   * @param data
   * @param condition
   * @return
   */
  private boolean whereCheckRowConditon(Data data,Condition condition)
    throws SQLException
  {
    Object left = condition.getLeft();
    Object right = condition.getRight();
    switch(condition.getOperator())
    {
      case Condition.AND:
        return whereCheckRow(data,left) && whereCheckRow(data,right);
      case Condition.OR:
        return whereCheckRow(data,left) || whereCheckRow(data,right);
      default:
        throw new RuntimeException("unknown condition operator: " + condition.getOperator());
    }
  }

  private Object resolveColumn(Data data,Object column)
    throws SQLException
  {
    if (column instanceof FQColumn)
    {
      FQColumn c = (FQColumn)column;
      return data.get(tableMap[c.getTable()]+c.getColumn());
    }
    else if (column instanceof FunctionDef)
    {
      FunctionDef fdef = (FunctionDef)column;
      Function f = (Function)fdef.getFunction();
      Object argument = fdef.getArgument();
      return f.execute(resolveColumn(data,argument));
    }
    else
      return column;  // static value
  }
}

