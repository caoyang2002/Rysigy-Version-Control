trait Overview{
    fn overview(&self) -> String {
        String::from("Course")
    }
}

trait Another {
    fn hell(&self) {
        println!("welcome to hell");
    }
}

// impl 和泛型写法的两种区别
struct Course {
    headline: String,
    author:String,
}

impl Overview for Course {}

struct AnotherCourse{
    headline: String,
    author: String,
}

// impl 的写法
impl Overview for AnotherCourse {}
fn call_overview(item:&impl Overview){
    println!("Overview {}",item.overview());
}

// 写法
fn call_overview_generic<T:Overview>(item:&T){
    println!("Overview {}",item.overview());
}

fn call_overview_T(item1:&impl Overview,item2:&impl Overview){
    println!("Overview {}",item1.overview());
    println!("Overview {}",item2.overview());
}

// 泛型
fn call_overview_TT<T:Overview>(item1:&T,item2:&T){
    println!("Overview {}",item1.overview());
    println!("Overview {}",item2.overview());
}
fn main(){
    let c0 = Course{headline:"headline".to_owned(),author:"author".to_owned(),};
    let c1 = Course{headline:"headline".to_owned(),author:"author".to_owned(),};
    let c2 = AnotherCourse{headline:"headline_2".to_owned(),author:"author_2".to_owned(),};
    call_overview(&c1);
    // Overview Course
    call_overview_generic(&c1);
    // Overview Course

    call_overview(&c1);
    // Overview Course
    call_overview_generic(&c2); // the trait `Overview` is not implemented for `AnotherCourse`
    // Overview Course
    call_overview_T(&c1,&c2);
    // Overview Course
    // Overview Course

    //
    println!("----------------------");
    // call_overview_TT(&c1,&c2); // &c2: the trait `Overview` is not implemented for `AnotherCourse`
    call_overview_TT(&c1,&c0);
    call_overview_T(&c1,&c0);
    // Overview Course
    // Overview Course

}