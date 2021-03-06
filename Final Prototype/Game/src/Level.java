import java.util.ArrayList;
import java.util.Random;

public class Level {
	
	int enem, collect;
	private ArrayList<Entity> entities;
	
	private Player player;
	
	public ArrayList<Entity> getEntities() {
		return entities;
	}

	public Level(int enem, Player player, int collect){
		entities = new ArrayList<Entity>();
		this.collect = collect;
		this.enem = enem;
		this.player = player;
		spawn(enem, EntityType.ENEMY);
		spawn(collect, EntityType.COLLECT);
	}
	
	public void spawn(int num, EntityType t){
		Random rand = new Random();
		for(int i = 0; i < enem; i++){
			int tx = rand.nextInt(MouseMotion.WIDTH - 100) + 10;
			int ty = rand.nextInt(100 - 0 + 1) + 0;
			if(t == EntityType.ENEMY)
				addEntity(new Enemy(tx,ty,1,1,20, player));
			else
				addEntity(new Collectible(tx,ty,1,1,20, player));
		}
	}
	
	public void addEntity(Entity e){
		entities.add(e);
	}
	
	public boolean respawn(EntityType t){
		int count = 0;
		for(Entity i : this.entities){
			if(i.getT() == t)count++;
		}
		if(count == 0) return true;
		return false;
	}
	
}
