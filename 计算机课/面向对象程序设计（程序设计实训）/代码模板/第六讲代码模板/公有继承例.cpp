class  Base
{
 private:
     int  base_pri_data;
 protected:
     int  base_pro_data;
 public:
     int  base_pub_data;
};

class  PublicDerived: public Base
{
private:
    int derived_pri_data;
protected:
    int derived_pro_data;
public:
    int derived_pub_data;
    void  testBase()
    {
       //base_pri_data = 1; //error: 基类 私有成员在公有继承的派生类中不可访问
       base_pro_data = 2; //OK，基类保护成员在公有继承的派生类中仍是受保护的
       base_pub_data = 3; //OK，基类公有成员在公有继承的派生类中仍是公有的
   }
};
int  main()
{
    PublicDerived   obj_of_public_derived_class;
               
    obj_of_public_derived_class.derived_pub_data = 1;   // ok
    obj_of_public_derived_class.testBase();             // ok
    //obj_of_public_derived_class.derived_pri_data = 2;   // error
    //obj_of_public_derived_class.derived_pro_data = 3;   // error
    obj_of_public_derived_class.base_pub_data = 1;      // ok
    //obj_of_public_derived_class.base_pri_data = 2;      // error
    //obj_of_public_derived_class.base_pro_data = 3;      // error
} 
